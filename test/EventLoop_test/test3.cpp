//
// test3.cpp
//
// Copyright (c) 2017 Jiawei Feng
//


#include "../../src/net/EventLoop.h"
#include "../../src/net/Channel.h"
#include "../../src/base/Thread.h"

#include <stdio.h>
#include <sys/timerfd.h>
#include <string.h>
#include <unistd.h>

Dalin::Net::EventLoop *g_loop;

void timeout(Dalin::Timestamp receiveTime)
{
    printf("%s Timeout!\n", receiveTime.toString().c_str());
    g_loop->quit();
}

int main()
{
    printf("%s started\n", Dalin::Timestamp::now().toString().c_str());
    Dalin::Net::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Dalin::Net::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
}
