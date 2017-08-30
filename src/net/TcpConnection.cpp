//
// TcpConnection.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

using namespace Dalin::Net;
using std::placeholders::_1;

TcpConnection::TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr)
 : loop_(loop),
   name_(name),
   state_(kConnecting),
   socket_(new Socket(sockfd)),
   channel_(new Channel(loop, sockfd)),
   localAddr_(localAddr),
   peerAddr_(peerAddr)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback([&](){ this->handleWrite(); });
    channel_->setCloseCallback([&](){ this->handleClose(); });
    channel_->setErrorCallback([&](){ this->handleError(); });
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const std::string &message)
{
    if (state_ == kConnected) {
        if (loop_->isInLoopThread()) {
            sendInLoop(message);
        }
        else {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string &message)
{
    loop_->assertInLoopThread();

    ssize_t nwrote = 0;
    // if nothing in ouput queue, try writing directly.
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        nwrote = ::write(channel_->fd(), message.data(), message.size());
        if (nwrote >= 0 ) {
            if (static_cast<size_t>(nwrote) < message.size()) {
                // will write more data
            }
            else if (writeCompleteCallback_) {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }
        else {
            nwrote = 0;
            if (errno != EWOULDBLOCK) {
                fprintf(stderr, "Failed in TcpConnection::sendInLoop()\n");
            }
        }
    }

    assert(nwrote >= 0);
    if (static_cast<size_t>(nwrote) < message.size()) {
        outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
        if (!channel_->isWriting()) {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown()
{
    if (state_ == kConnected) {
        setState(kDisconnecting);
        loop_->runInLoop([&](){ this->shutdownInLoop(); });
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if (!channel_->isWriting()) {
        socket_->shutdownWrite();
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);

    setState(kConnected);
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected || state_ == kDisconnecting);

    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(channel_.get());
}

void TcpConnection::handleRead(Timestamp receive)
{
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);

    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_, n);
    }
    else if (n == 0 ) {
        handleClose();
    }
    else {
        errno = savedErrno;
        fprintf(stderr, "Failed in TcpConnection::handleRead\n");
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();

    if (channel_->isWriting()) {
        ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if (n > 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }

                if (state_ == kDisconnecting) {
                    shutdownInLoop();
                }
            }
            else {
                // will write more data
            }
        }
        else {
            fprintf(stderr, "Failed in TcpConnection::handleWrite()\n");
            abort();
        }
    }
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected || state_ == kDisconnecting);

    // we don't close fd, leave it to dtor, in order to find leaks easily.
    channel_->disableAll();

    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    int err = SocketsOps::getSocketError(channel_->fd());
    fprintf(stderr, "TcpConnection::handleError(): [%s] - SO_ERROR = %d\n", name_.c_str(), err);
}
