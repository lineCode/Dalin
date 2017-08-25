//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/base/ThreadPool.h"
#include "../../src/base/CountDownLatch.h"
#include "../../src/base/CurrentThread.h"
#include <stdio.h>
#include <string>
#include <unistd.h>

void print()
{
    printf("current thread = %d\n", Dalin::CurrentThread::tid());
}

void printString(const std::string taskId)
{
    printf("current thread = %d %s\n",
        Dalin::CurrentThread::tid(),taskId.c_str());
    usleep(50 * 1000);
}

void test(int maxSize)
{
    printf("Test ThreadPool with max queue size = %d\n", maxSize);
    Dalin::ThreadPool pool("Main ThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    printf("Adding print task\n");
    pool.addTask(print);
    pool.addTask(print);

    printf("Adding printString task\n");
    for (int i = 0; i != 100; ++i) {
        std::string taskId("task - ");
        taskId += std::to_string(i);
        pool.addTask([=](){ printString(taskId); });
    }

    Dalin::CountDownLatch latch(1);
    pool.addTask([&](){ latch.countDown(); });
    latch.wait();
    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}
