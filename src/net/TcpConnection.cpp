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
    channel_->setReadCallback([&](){ this->handleRead(); });
    channel_->setWriteCallback([&](){ this->handleWrite(); });
    channel_->setCloseCallback([&](){ this->handleClose(); });
    channel_->setErrorCallback([&](){ this->handleError(); });
}

TcpConnection::~TcpConnection()
{

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
    assert(state_ == kConnected);

    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(channel_.get());
}

void TcpConnection::handleRead()
{
    char buf[65536];
    ssize_t n = ::read(channel_->fd(), buf, sizeof(buf));

    if (n > 0) {
        messageCallback_(shared_from_this(), buf, n);
    }
    else if (n == 0 ) {
        handleClose();
    }
    else {
        handleError();
    }
}

void TcpConnection::handleWrite()
{

}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected);

    // we don't close fd, leave it to dtor, in order to find leaks easily.
    channel_->disableAll();

    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    int err = SocketsOps::getSocketError(channel_->fd());
    fprintf(stderr, "TcpConnection::handleError(): [%s] - SO_ERROR = %d\n", name_.c_str(), err);
}
