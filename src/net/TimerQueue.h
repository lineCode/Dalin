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


namespace Xgeer {
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

    // void cancel(TimerId timerId); // TODO

private:
    typedef std::pair<Timestamp, std::unique_ptr<Timer>> Entry;
    typedef std::set<Entry> TimerList;

    void addTimerInLoop(Timer *timer);
    // called when timerfd alarms
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(std::vector<Entry> &expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop *loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    // Timer list sorted by expiration
    TimerList timers_;
};

}
}

#endif
