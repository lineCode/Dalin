//
// test3.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/Acceptor.h"
#include "../../src/net/EventLoop.h"
#include "../../src/net/InetAddress.h"
#include "../../src/net/TcpServer.h"

#include <stdio.h>
#include <unistd.h>
#include <string>

std::string g_message1;
std::string g_message2;

void onConnection(const Dalin::Net::TcpConnectionPtr &conn)
{
    if (conn->connected()) {
        printf("onConnection(): tid = %d new connection [%s] from %s\n",
                Dalin::CurrentThread::tid(),
                conn->name().c_str(),
                conn->peerAddress().toHostPort().c_str());

        if (!g_message1.empty()) {
            conn->send(g_message1);
        }
        if (!g_message2.empty()) {
            conn->send(g_message2);
        }

        conn->shutdown();
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

    buf->retrieveAll();
}

int main(int argc, char *argv[])
{
    printf("main(): pid = %d\n", getpid());

    int len1 = 100;
    int len2 = 200;

    if (argc > 2) {
        len1 = atoi(argv[1]);
        len2 = atoi(argv[2]);
    }

    g_message1.resize(len1);
    g_message2.resize(len2);
    std::fill(g_message1.begin(), g_message1.end(), 'A');
    std::fill(g_message2.begin(), g_message2.end(), 'B');

    Dalin::Net::InetAddress listenAddr(9981);
    Dalin::Net::EventLoop loop;

    Dalin::Net::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    if (argc > 3) {
        server.setThreadNum(atoi(argv[3]));
    }
    server.start();

    loop.loop();
}
