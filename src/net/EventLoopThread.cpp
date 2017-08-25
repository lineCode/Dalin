//
// EventLoopThread.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "EventLoopThread.h"
#include "EventLoop.h"

using namespace Dalin::Net;

EventLoopThread::EventLoopThread()
 : loop_(NULL),
   exiting_(false),
   thread_([&](){ this->threadFunc(); }),
   mutex_(),
   cond_(mutex_)
{

}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    loop_->quit();
    thread_.join();
}

EventLoop *EventLoopThread::startLoop()
{
    assert(!thread_.started());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while (loop_ == NULL) {
            cond_.wait();
        }
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }

    loop.loop();
}
