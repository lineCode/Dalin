//
// Acceptor.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Socket.h"
#include "Channel.h"

namespace Dalin {
namespace Net {

class EventLoop;
class InetAddress;

// Acceptor of incoming TCP connections.
class Acceptor : Noncopyable {
public:
    typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop *loop, const InetAddress &listenAddr);

    void setNewConnectionCallback(const NewConnectionCallback &cb)
    {
        newConnectionCallback_ = cb;
    }

    bool listenning() const { return listenning_; }
    void listen();

private:
    void handleRead();

    EventLoop *loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
};

}
}

#endif
