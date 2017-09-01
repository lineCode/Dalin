//
// Timer.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Timer.h"

namespace Dalin {
namespace Net {

std::atomic<int64_t> Timer::s_numCreated_;

void Timer::restart(Timestamp now)
{
    if (repeat_) {
        expiration_ = addTime(now, interval_);
    }
    else {
        expiration_ = Timestamp::invaild();
    }
}

}
}
