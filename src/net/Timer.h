//
// Timer.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TIMER_H
#define TIMER_H

#include "../base/Noncopyable.h"
#include "../base/Timestamp.h"
#include "Callbacks.h"

#include <atomic>

namespace Dalin {
namespace Net {

// Internal class for timer event.
class Timer : Noncopyable {
public:
    Timer(const TimerCallback &cb, Timestamp when, double interval)
     : callback_(cb),
       expiration_(when),
       interval_(interval),
       repeat_(interval > 0.0),
       sequence_(++s_numCreated_)
     {

     }

     void run() const
     {
         callback_();
     }

     Timestamp expiration() const { return expiration_; }
     bool repeat() const { return repeat_; }
     int64_t sequence() const { return sequence_; }

     void restart(Timestamp now);

private:
    const TimerCallback callback_;
    Timestamp expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;

    static std::atomic<int64_t> s_numCreated_;
};

}
}

#endif
