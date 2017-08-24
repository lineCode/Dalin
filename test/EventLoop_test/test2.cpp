//
// test2.cpp
//
// Copyright (c) 2017 Jiawei Feng
//


#include "../../src/net/EventLoop.h"
#include "../../src/base/Thread.h"

Xgeer::Net::EventLoop *g_loop;

void threadFunc()
{
    g_loop->loop();
}

int main()
{
    Xgeer::Net::EventLoop loop;
    g_loop = &loop;

    Xgeer::Thread thread(threadFunc);
    thread.start();
    thread.join();
}
