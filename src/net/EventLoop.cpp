//
// EventLoop.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "EventLoop.h"
#include <assert.h>
#include <poll.h>
#include <stdio.h>
#include "Poller.h"
#include "Channel.h"

using namespace Xgeer;
using namespace Xgeer::Net;

__thread EventLoop *t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
 : looping_(false),
   quit_(false),
   tid_(CurrentThread::tid()),
   poller_(new Poller(this))
{
    if (t_loopInThisThread) {
        fprintf(stderr, "Another EventLoop %p exists in this thread %d\n", t_loopInThisThread, tid_);
        abort();
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
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();

        poller_->poll(kPollTimeMs, &activeChannels_);

        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
    }

    looping_ = false;
}

void EventLoop::abortNotInLoopThread()
{
    fprintf(stderr,
        "FATAL: EventLoop::abortNotInLoopThread - EventLoop %p was created in thread id = %d,\
        current thread id = %d\n", this, tid_, CurrentThread::tid());
    abort();
}

void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();

    poller_->updateChannel(channel);
}
