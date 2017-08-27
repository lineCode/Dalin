//
// TcpConnection.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TCPCONNETION_H
#define TCPCONNETION_H

#include "Callbacks.h"
#include "InetAddress.h"
#include "../base/Noncopyable.h"

#include <memory>
#include <string>

namespace Dalin {
namespace Net {

class Channel;
class EventLoop;
class Socket;

// TCP connection, for both client and server usage.
class TcpConnection : Noncopyable, std::enable_shared_from_this<TcpConnection> {
public:
    // Constructs a TcpConnection with a connected sockfd.
    // User should not create this object.
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);
    ~TcpConnection();

    EventLoop *getLoop() const { return loop_; }
    const std::string &name() const { return name_; }
    const InetAddress &localAddress() { return localAddr_; }
    const InetAddress &peerAddress() { return peerAddr_; }

    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback &cb)
    {
        messageCallback_ = cb;
    }

    // Internal use only.
    // called when Tcpserver accepts a new connection.
    // should be called only once.
    void connectEstablished();

private:
    enum stateE { kConnecting, kConnected };

    void setState(stateE s) { state_ = s; }
    void handleRead();

    EventLoop *loop_;
    std::string name_;
    stateE state_;

    //
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
};

}
}


#endif
