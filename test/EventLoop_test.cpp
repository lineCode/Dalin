//
// EventLoop_test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include <sys/timerfd.h>
#include "../src/net/EventLoop.h"
#include "../src/net/Poller.h"
#include "../src/net/Channel.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace Xgeer::Net;

EventLoop *g_loop;

void timeout()
{
    printf("Timeout\n");
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    ::bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
}
