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
class EventLoopThreadPool;

class TcpServer : Noncopyable {
public:
    TcpServer(EventLoop *loop, const InetAddress &listenAddr);
    ~TcpServer();

    // Set the number of threads for handling input.
    // Always accepts new connection in loop's thread.
    // Must be called before start()!!!
    // @numThreads
    // - 0, means all I/O in loop thread, no thread will be created. Default value.
    // - 1, means all I/O in another thread.
    // - N, means a thread pool with N threads, new connections are assigned.
    void setThreadNum(int numThreads);

    // Starts the server if it's not listening.
    // Thread safe.
    void start();

    // Set connection callback.
    // Not thread safe.
    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }

    // Set Message callback.
    // Not thread safe.
    void setMessageCallback(const MessageCallback &cb)
    {
        messageCallback_ = cb;
    }

    // Set Write complete callback.
    // Not thread safe.
    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    {
        writeCompleteCallback_ = cb;
    }

private:
    // Not thread safe, but in loop.
    void newConnection(int sockfd, const InetAddress &peerAddr);
    // Thread safe
    void removeConnection(const TcpConnectionPtr &conn);
    // Not thread safe, but in loop
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop *loop_; // the acceptor loop
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::unique_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};

}
}

#endif
