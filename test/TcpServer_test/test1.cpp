//
// test1.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/Acceptor.h"
#include "../../src/net/EventLoop.h"
#include "../../src/net/InetAddress.h"
#include "../../src/net/TcpServer.h"

#include <stdio.h>
#include <unistd.h>

void onConnection(const Dalin::Net::TcpConnectionPtr &conn)
{
    if (conn->connected()) {
        printf("onConnection(): tid = %d new connection [%s] from %s\n",
                Dalin::CurrentThread::tid(),
                conn->name().c_str(),
                conn->peerAddress().toHostPort().c_str());
    }
    else {
        printf("onConnection(): tid = %d connection [%s] is down\n",
                Dalin::CurrentThread::tid(),
                conn->name().c_str());
    }
}

void onMessage(const Dalin::Net::TcpConnectionPtr &conn,
               Dalin::Net::Buffer *buf,
               Dalin::Timestamp receiveTime)
{
    printf("onMessage(): tid = %d received %zd bytes from connection [%s] at %s\n",
            Dalin::CurrentThread::tid(),
            buf->readableBytes(),
            conn->name().c_str(),
            receiveTime.toString().c_str());

    printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main(int argc, char *argv[])
{
    printf("main(): pid = %d\n", getpid());

    Dalin::Net::InetAddress listenAddr(9981);
    Dalin::Net::EventLoop loop;

    Dalin::Net::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    if (argc > 1) {
        server.setThreadNum(atoi(argv[1]));
    }
    server.start();

    loop.loop();
}
