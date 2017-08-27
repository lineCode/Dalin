//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/Acceptor.h"
#include "../../src/net/EventLoop.h"
#include "../../src/net/InetAddress.h"
#include "../../src/net/SocketsOps.h"

#include <stdio.h>
#include <unistd.h>

void newConnection(int sockfd, const Dalin::Net::InetAddress &peerAddr)
{
    printf("newConnection(): accepted a new connection from %s\n", peerAddr.toHostPort().c_str());
    ::write(sockfd, "Hello world!\n", 14);

    Dalin::SocketsOps::close(sockfd);
}

int main()
{
    printf("main(): pid = %d\n", getpid());

    Dalin::Net::InetAddress listenAddr(9981);
    Dalin::Net::EventLoop loop;

    Dalin::Net::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();

    loop.loop();
}
