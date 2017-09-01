//
// test.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "../../src/net/EventLoop.h"
#include "../../src/base/Timestamp.h"

#include <stdio.h>
#include <unistd.h>
#include <string>

int cnt = 0;
Dalin::Net::EventLoop *g_loop;

void printTid()
{
    printf("pid = %d, tid = %d\n", getpid(), Dalin::CurrentThread::tid());
    printf("now %s\n", Dalin::Timestamp::now().toString().c_str());
}

void print(const char *msg)
{
    printf("msg %s %s\n", Dalin::Timestamp::now().toString().c_str(), msg);
    if (++cnt == 20) {
        g_loop->quit();
    }
}

Dalin::Net::TimerId toCancel;
void cancelSelf()
{
    print("cancelSelf()");
    g_loop->cancel(toCancel);
}

int main()
{
    printTid();

    Dalin::Net::EventLoop loop;
    g_loop = &loop;

    print("main");

    loop.runAfter(1, [](){ print("once1"); });
    loop.runAfter(1.5, [](){ print("once1.5"); });
    loop.runAfter(2.5, [](){ print("once2.5"); });
    loop.runAfter(3.5, [](){ print("once3.5"); });

    Dalin::Net::TimerId t = loop.runEvery(2, [](){ print("every2"); });
    loop.runEvery(3, [](){ print("every3"); });
    loop.runEvery(10, std::bind(&Dalin::Net::EventLoop::cancel, &loop, t));
    toCancel = loop.runEvery(5, cancelSelf);

    loop.loop();
    print("main loop exits");
    sleep(1);
}
