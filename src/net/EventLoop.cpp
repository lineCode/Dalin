//
// EventLoop.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"

#include <assert.h>
#include <sys/eventfd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

using namespace Dalin;
using namespace Dalin::Net;

__thread EventLoop *t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

namespace Dalin {
namespace Detail {

int createEventfd()
{
    int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (eventfd < 0) {
        fprintf(stderr, "Failed in eventfd\n");
        abort();
    }

    return eventfd;
}

class IgnoreSigPipe {
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE, SIG_IGN);
    }
};

}
}

Dalin::Detail::IgnoreSigPipe initObj;

EventLoop::EventLoop()
 : looping_(false),
   quit_(false),
   callingPendingFunctors_(false),
   threadId_(CurrentThread::tid()),
   poller_(new Poller(this)),
   timerQueue_(new TimerQueue(this)),
   wakeupFd_(Detail::createEventfd()),
   wakeupChannel_(new Channel(this, wakeupFd_))
{
    if (t_loopInThisThread) {
        fprintf(stderr, "Another EventLoop %p exists in this thread %d\n", t_loopInThisThread, threadId_);
        abort();
    }
    else {
        t_loopInThisThread = this;
    }

    //wakeupChannel_->setReadCallback([&](){ this->handleRead(); });
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    ::close(wakeupFd_);
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

        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);

        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent(pollReturnTime_);
        }

        doPendingFunctors();
    }

    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;

    if (!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::runInLoop(const Functor &cb)
{
    if (isInLoopThread()) {
        cb();
    }
    else {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor &cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }

    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::abortNotInThread()
{
    fprintf(stderr, "EventLoop::abortNotInThread - EventLoop %p \
            was created in threadId_ %d, current thread id = %d\n", \
            this, threadId_, CurrentThread::tid());
    abort();
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        fprintf(stderr, "EventLoop::wakeup() writes %ld bytes instead of 8\n", n);
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();

    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();

    poller_->removeChannel(channel);
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        fprintf(stderr, "EventLoop::handleRead() reads %ld bytes instead of 8\n", n);
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (size_t i = 0; i != functors.size(); ++i) {
        functors[i]();
    }

    callingPendingFunctors_ = false;
}

TimerId EventLoop::runAt(const Timestamp &time, const TimerCallback &cb)
{
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback &cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback &cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
    return timerQueue_->cancel(timerId);
}
