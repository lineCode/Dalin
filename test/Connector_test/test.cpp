//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/EventLoop.h"
#include "../../src/net/Connector.h"

#include <stdio.h>

Dalin::Net::EventLoop *g_loop;

void connectCallback(int sockfd)
{
    printf("Connected.\n");
    g_loop->quit();
}

int main(int argc, char *argv[])
{
    Dalin::Net::EventLoop loop;
    g_loop = &loop;

    Dalin::Net::InetAddress addr("127.0.0.1", 9981);

    Dalin::Net::ConnectorPtr connector(new Dalin::Net::Connector(&loop, addr));
    connector->setNewConnectionCallback(connectCallback);
    connector->start();

    loop.loop();
}
