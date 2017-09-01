//
// Connector.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Connector.h"
#include "Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <errno.h>
#include <string.h>

using namespace Dalin::Net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop *loop, const InetAddress &serverAddr)
 : loop_(loop),
   serverAddr_(serverAddr),
   connect_(false),
   state_(kDisconnected),
   retryDaleyMs_(kInitRetryDelayMs)
{

}

Connector::~Connector()
{
    loop_->cancel(timerId_);
}

void Connector::start()
{
    connect_ = true;
    loop_->runInLoop([&](){ this->startInLoop(); });
}

void Connector::startInLoop()
{
    loop_->assertInLoopThread();
    assert(state_ == kDisconnected);

    if (connect_) {
        connect();
    }
    else {
        fprintf(stderr, "Do not connect\n");
    }
}

void Connector::connect()
{
    int sockfd = SocketsOps::createNonblockingOrDie();
    int ret = SocketsOps::connect(sockfd, serverAddr_.getSockAddrInet());

    int savedErrno = (ret == 0) ? 0 : errno;
    switch (savedErrno) {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        connecting(sockfd);
        break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        retry(sockfd);
        break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        fprintf(stderr, "Failed in Connector::startInLoop(), errno = %d\n", savedErrno);
        SocketsOps::close(sockfd);
        break;

    default:
        fprintf(stderr, "Failed in Connector::startInLoop(), unexpected errno = %d\n", savedErrno);
        SocketsOps::close(sockfd);
        break;
    }
}

void Connector::restart()
{
    loop_->assertInLoopThread();

    setState(kDisconnected);
    retryDaleyMs_ = kInitRetryDelayMs;
    connect_ = true;

    startInLoop();
}

void Connector::stop()
{
    connect_ = false;

    loop_->cancel(timerId_);
}

void Connector::connecting(int sockfd)
{
    setState(kConnecting);

    assert(!channel_);
    channel_.reset(new Channel(loop_, sockfd));
    channel_->setWriteCallback([&](){ this->handleWrite(); });
    channel_->setErrorCallback([&](){ this->handleError(); });
    channel_->enableWriting();
}

int Connector::removeAndResetChannel()
{
    channel_->disableAll();
    loop_->removeChannel(channel_.get());

    int sockfd = channel_->fd();
    // Can't reset channel_ here, because we are inside Channel::handleEvent
    loop_->queueInLoop([&](){ this->resetChannel(); });

    return sockfd;
}

void Connector::resetChannel()
{
    channel_.reset();
}

void Connector::handleWrite()
{
    if (state_ == kConnecting) {
        int sockfd = removeAndResetChannel();
        int err = SocketsOps::getSocketError(sockfd);
        if (err) {
            fprintf(stderr, "Connector::handleWrite - SO_ERROR = %d %s\n", err, strerror(err));
            retry(sockfd);
        }
        else if (SocketsOps::isSelfConnect(sockfd)) {
            fprintf(stderr, "Connector::handleWrite - Self connect\n");
            retry(sockfd);
        }
        else {
            setState(kConnected);
            if (connect_) {
                newConnectionCallback_(sockfd);
            }
            else {
                SocketsOps::close(sockfd);
            }
        }
    }
    else {
        // what happened?
        assert(state_ == kDisconnected);
    }
}

void Connector::handleError()
{
    fprintf(stderr, "Failed in Connector::handleError()\n");
    assert(state_ == kConnecting);

    int sockfd = removeAndResetChannel();
    int err = SocketsOps::getSocketError(sockfd);
    fprintf(stderr, "SO_ERROR = %d %s\n", err, strerror(err));

    retry(sockfd);
}

void Connector::retry(int sockfd)
{
    SocketsOps::close(sockfd);
    setState(kDisconnected);

    if (connect_) {
        timerId_ = loop_->runAfter(retryDaleyMs_ / 1000.0, [&](){ this->startInLoop(); });

        retryDaleyMs_ = std::min(retryDaleyMs_ * 2, kMaxRetryDelayMs);
    }
    else {
        fprintf(stderr, "do not connect\n");
    }
}
