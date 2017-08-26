//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/EventLoopThread.h"
#include "../../src/net/EventLoop.h"

#include <stdio.h>
#include <unistd.h>

void runInThread()
{
    printf("runInThread(): pid = %d, tid = %d\n", getpid(), Dalin::CurrentThread::tid());
}

int main()
{
    printf("main(): pid = %d, tid = %d\n", getpid(), Dalin::CurrentThread::tid());

    Dalin::Net::EventLoopThread loopThread;
    Dalin::Net::EventLoop *loop = loopThread.startLoop();

    loop->runInLoop(runInThread);
    sleep(1);
    loop->runAfter(2, runInThread);
    sleep(3);
    loop->quit();

    printf("exit main()\n");
}
