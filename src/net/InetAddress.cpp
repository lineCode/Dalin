//
// InetAddress.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "InetAddress.h"
#include "SocketsOps.h"

#include <strings.h>
#include <netinet/in.h>

using namespace Dalin::Net;

static const in_addr_t kInaddrAny = INADDR_ANY;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in),
                "sizeof(InetAddress) != sizeof(struct sockaddr_in)");


InetAddress::InetAddress(uint16_t port)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = SocketsOps::hostToNetwork32(kInaddrAny);
    addr_.sin_port = SocketsOps::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    bzero(&addr_, sizeof(addr_));
    SocketsOps::fromHostPort(ip.c_str(), port, &addr_);
}

std::string InetAddress::toHostPort() const
{
    char buf[32];
    SocketsOps::toHostPort(buf, sizeof(buf), addr_);

    return buf;
}
