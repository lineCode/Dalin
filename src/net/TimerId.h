//
// TimerId.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TIMERID_H
#define TIMERID_H

#include <stdint.h>
#include <stdlib.h>

namespace Dalin {
namespace Net {

class Timer;

class TimerId {
public:
    explicit TimerId(Timer *timer = NULL, int64_t seq = 0)
     : value_(timer),
       sequence_(seq)
    {

    }

    friend class TimerQueue;

private:
    Timer *value_;
    int64_t sequence_;
};

}
}

#endif
