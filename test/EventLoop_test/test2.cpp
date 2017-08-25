//
// test2.cpp
//
// Copyright (c) 2017 Jiawei Feng
//


#include "../../src/net/EventLoop.h"
#include "../../src/base/Thread.h"

Dalin::Net::EventLoop *g_loop;

void threadFunc()
{
    g_loop->loop();
}

int main()
{
    Dalin::Net::EventLoop loop;
    g_loop = &loop;

    Dalin::Thread thread(threadFunc);
    thread.start();
    thread.join();
}
