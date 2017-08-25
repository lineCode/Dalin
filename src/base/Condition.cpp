//
// Condition.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Condition.h"
#include <sys/time.h>
#include <errno.h>


// return true if time out, false otherwise.
bool Dalin::Condition::waitForSeconds(double seconds)
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);

    const int64_t kNanoSecondPerSecond = 1e9;
    int64_t nanoSeconds = static_cast<int64_t>(seconds * kNanoSecondPerSecond);

    time.tv_sec += static_cast<time_t>((time.tv_nsec + nanoSeconds) / kNanoSecondPerSecond);
    time.tv_nsec = static_cast<long>((time.tv_nsec + nanoSeconds) % kNanoSecondPerSecond);

    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.getPthreadMutex(), &time);
}
