//
// Socket.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Socket.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>

using namespace Dalin;
using namespace Dalin::Net;

Socket::~Socket()
{
    SocketsOps::close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localAddr)
{
    SocketsOps::bindOrDie(sockfd_, localAddr.getSockAddrInet());
}

void Socket::listen()
{
    SocketsOps::listenOrdie(sockfd_);
}

int Socket::accept(InetAddress *peerAddr)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    int connfd = SocketsOps::accept(sockfd_, &addr);
    if (connfd >= 0 ) {
        peerAddr->setSockAddrInet(addr);
    }

    return connfd;
}

void Socket::setReuseAddr(bool on)
{
    int optVal = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
}

void Socket::shutdownWrite()
{
    SocketsOps::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
    int optVal = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optVal, sizeof(optVal));
}
