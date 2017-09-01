//
// EventLoop.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "../base/Timestamp.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Noncopyable.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <memory>
#include <vector>

namespace Dalin {
namespace Net {

class Channel;
class Poller;
class TimerQueue;

class EventLoop : Noncopyable {
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    // Loops forever.
    // Must be called in the same thread as creation of the object.
    void loop();

    void quit();

    // Time when poll returns
    Timestamp pollReturnTime() const { return pollReturnTime_; }

    // Run callback immediately in the loop thread.
    // It wakes up the loop, and run the cb.
    void runInLoop(const Functor &cb);

    // Queues callback in the loop thread.
    // Runs after finish pooling.
    // Safe to call from other threads.
    void queueInLoop(const Functor &cb);

    // Timers
    // Safe to call from other threads.

    // Runs callback at time.
    TimerId runAt(const Timestamp &time, const TimerCallback &cb);
    // Runs callback after delay seconds.
    TimerId runAfter(double delay, const TimerCallback &cb);
    // Runs callback every interval seconds.
    TimerId runEvery(double interval, const TimerCallback &cb);

    void cancel(TimerId timerId);

    // internal use only
    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);

    void assertInLoopThread()
    {
        if (!isInLoopThread()) {
            abortNotInThread();
        }
    }

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    void abortNotInThread();
    void handleRead();
    void doPendingFunctors();

    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    bool quit_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    ChannelList activeChannels_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_; // Visable to other threads, guarded by mutex_
};

}
}

#endif
