//
// EventLoopThreadPool.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

using namespace Dalin::Net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop)
 : baseLoop_(baseLoop),
   started_(false),
   numThreads_(0),
   next_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
    // Don't delete baseLoop_ and loops_, they're stack variables.
}

void EventLoopThreadPool::start()
{
    assert(!started_);
    baseLoop_->assertInLoopThread();

    started_ = true;

    for (int i = 0; i < numThreads_; ++i) {
        auto t = std::unique_ptr<EventLoopThread>(new EventLoopThread);
        auto loop = t->startLoop();
        threads_.push_back(std::move(t));
        loops_.push_back(loop);
    }
}

EventLoop *EventLoopThreadPool::getNextLoop()
{
    baseLoop_->assertInLoopThread();

    EventLoop *loop = baseLoop_;

    if (!loops_.empty()) {
        loop = loops_[next_];

        ++next_;
        if (static_cast<size_t>(next_) >= loops_.size()) {
            next_ = 0;
        }
    }

    return loop;
}
