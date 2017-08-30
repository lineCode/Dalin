//
// Socket.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef SOCKET_H
#define SOCKET_H

#include "../base/Noncopyable.h"

namespace Dalin {
namespace Net {

class InetAddress;

class Socket : Noncopyable {
public:
    explicit Socket(int sockfd)
     : sockfd_(sockfd)
    {

    }

    ~Socket();

    int fd() const { return sockfd_; }

    // abort if address in use
    void bindAddress(const InetAddress &localAddr);
    // abort if address in use
    void listen();

    // On success, returns a descriptor for the accepted socket, which has been
    // set to non-blocking and close-on-exec. *peerAddr is assigned.
    // On error, -1 is returned, and *peerAddr is untouched.
    int accept(InetAddress *peerAddr);

    // Enable / disable SO_REUSEADDR
    void setReuseAddr(bool on);

    void shutdownWrite();

    // Enable / disable TCP_NODELAY
    void setTcpNoDelay(bool on);

private:
    const int sockfd_;
};

}
}

#endif
