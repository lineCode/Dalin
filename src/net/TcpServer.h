//
// TcpServer.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Callbacks.h"
#include "TcpConnection.h"
#include "../base/Noncopyable.h"

#include <map>
#include <memory>
#include <string>

namespace Dalin {
namespace Net {

class Acceptor;
class EventLoop;

class TcpServer : Noncopyable {
public:
    TcpServer(EventLoop *loop, const InetAddress &listenAddr);
    ~TcpServer();

    // Starts the server if it's not listening.
    // Thread safe.
    void start();

    // Set connection callback.
    // Not thread safe.
    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback &cb)
    {
        messageCallback_ = cb;
    }

private:
    // Not thread safe, but in loop.
    void newConnection(int sockfd, const InetAddress &peerAddr);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop *loop_; // the acceptor loop
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};

}
}

#endif
