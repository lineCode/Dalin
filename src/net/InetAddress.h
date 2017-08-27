//
// InetAddress.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>

namespace Dalin {
namespace Net {

// Wrapper of sockaddr_in
class InetAddress {
public:
    // Constructs an endpoint with given port number.
    // Mostly used in Tcpserver listening.
    explicit InetAddress(uint16_t port);

    // Constructs an endpoint with given ip and port.
    // ip should be "1.2.3.4".
    InetAddress(const std::string &ip, uint16_t port);

    // Constructs and endpoint with given struct sockaddr_in.
    // Mostly used when accepting nwe connections.
    InetAddress(const struct sockaddr_in &addr)
     : addr_(addr)
    {

    }

    std::string toHostPort() const;

    const struct sockaddr_in &getSockAddrInet() const { return addr_; }
    void setSockAddrInet(const struct sockaddr_in &addr) { addr_ = addr; }

private:
    struct sockaddr_in addr_;
};

}
}

#endif
