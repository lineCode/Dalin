//
// TcpServer.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "TcpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <stdio.h>

using std::placeholders::_1;

namespace {

Dalin::Net::EventLoop *CHECK_NOTNULL(Dalin::Net::EventLoop *loop)
{
    if (loop == NULL) {
        fprintf(stderr, "Failed in CHECK_NOTNULL(EventLoop)\n");
        abort();
    }
    return loop;
}

}

using namespace Dalin::Net;
using std::placeholders::_1;
using std::placeholders::_2;

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr)
 : loop_(CHECK_NOTNULL(loop)),
   name_(listenAddr.toHostPort()),
   acceptor_(new Acceptor(loop, listenAddr)),
   started_(false),
   nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    if (!started_) {
        started_ = true;
    }

    if (!acceptor_->listenning()) {
        loop_->runInLoop([&](){ acceptor_->listen(); });
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    loop_->assertInLoopThread();

    char buf[32];
    snprintf(buf, sizeof(buf), "#%d", nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    InetAddress localAddr(SocketsOps::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback([&](const TcpConnectionPtr &conn){ this->removeConnection(conn); });
    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->assertInLoopThread();

    size_t n = connections_.erase(conn->name());
    assert(n == 1);

    loop_->queueInLoop([&](){ conn->connectDestroyed(); });
}
