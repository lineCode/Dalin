//
// test1.cpp
//
// Copyright (c) 2017 Jiawei Feng
//


#include "../../src/net/EventLoop.h"
#include "../../src/base/Thread.h"

#include <stdio.h>
#include <unistd.h>


void threadFunc()
{
    printf("threadFunc(): pid = %d, tid = %d\n", getpid(), Xgeer::CurrentThread::tid());

    Xgeer::Net::EventLoop loop;
    loop.loop();
}

int main()
{
    printf("main(): pid = %d, tid = %d\n", getpid(), Xgeer::CurrentThread::tid());

    Xgeer::Net::EventLoop loop;

    Xgeer::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    pthread_exit(NULL);
}
