//
// Timer.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Timer.h"

namespace Xgeer {
namespace Net {

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
