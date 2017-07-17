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
#include "Channel.h"

namespace Xgeer {
namespace Net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : Noncopyable {
public:
    TimerQueue(EventLoop *loop);
    
private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    EventLoop *loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;  // Timer list sorted by expiration
};

}
}

#endif
