//
// Timer.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Timer.h"

using namespace Xgeer::Detail;

void Timer::restart(Xgeer::Timestamp now)
{
    if (repeat_) {
        expiration_ = addTime(now, interval_); // ../base/Timestamp.h
    }
    else {
        expiration_ = Timestamp::invalid();
    }
}
