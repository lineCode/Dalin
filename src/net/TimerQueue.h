//
// TimerQueue.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <set>
#include <vector>
#include <memory>

#include "../base/Noncopyable.h"
#include "../base/Timestamp.h"
#include "../base/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"


namespace Dalin {
namespace Net {

class EventLoop;
class Timer;
class TimerId;

// Timer queue.
class TimerQueue : Noncopyable {
public:
    TimerQueue(EventLoop *loop);
    ~TimerQueue();

    // Schedules the callback to run at the given time,
    // repeats if interval > 0.0.
    // Must be thread safe, usually be called from other threads.
    TimerId addTimer(const TimerCallback &cb, Timestamp when, double interval);

    void cancel(TimerId timerId);

private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer *, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer *timer);
    void cancelInLoop(TimerId timerId);
    // called when timerfd alarms
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(std::vector<Entry> &expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop *loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_; // Timer list sorted by expiration

    // for cancel()
    bool callingExpiredTimers_; // atomic
    ActiveTimerSet activeTimers_;
    ActiveTimerSet cancelingTimers_;
};

}
}

#endif
