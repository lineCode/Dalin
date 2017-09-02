//
// TcpClient.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "../base/Mutex.h"
#include "../base/Noncopyable.h"
#include "../net/TcpConnection.h"

namespace Dalin {
namespace Net {

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : Noncopyable {
public:
    TcpClient(EventLoop *loop, const InetAddress &serverAddr);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    bool retry() const;

    void enableRetry()
    {
        retry_ = true;
    }

    // Set connection callback
    // Not thread safe.
    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }

    // Set message callback
    // Not thread safe.
    void setMessageCallback(const MessageCallback &cb)
    {
        messageCallback_ = cb;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    {
        writeCompleteCallback_ = cb;
    }

private:
    // Not thread safe, but in loop
    void newConnection(int sockfd);
    // Not thread safe, bu in loop
    void removeConnection(const TcpConnectionPtr &conn);

    EventLoop *loop_;
    ConnectorPtr connector_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;
    bool connect_;
    // always in loop thread
    int nextConnId_;
    mutable MutexLock mutex_;
    TcpConnectionPtr connection_; // Guarded by mutex_
};

}
}

#endif
