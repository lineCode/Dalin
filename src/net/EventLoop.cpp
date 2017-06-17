//
// EventLoop.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "EventLoop.h"
#include <assert.h>
#include <poll.h>
#include <stdio.h>

using namespace Xgeer;
using namespace Xgeer::Net;

__thread EventLoop *t_loopInThisThread = 0;

EventLoop::EventLoop()
 : looping_(false),
   tid_(CurrentThread::tid())
{
    if (t_loopInThisThread) {
        fprintf(stderr, "Another EventLoop %p exists in this thread %d",
            t_loopInThisThread, tid_);
    }
    else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;

    ::poll(NULL, 0, 5 * 1000);

    looping_ = false;
}

void EventLoop::abortNotInLoopThread()
{
    fprintf(stderr, "FATAL: EventLoop::abortNotInLoopThread - EventLoop %p was created in thread id = %d,\
        current thread id = %d\n", this, tid_, CurrentThread::tid());
}
