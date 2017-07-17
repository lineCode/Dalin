//
// TimerQueue.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "TimerQueue.h"

#include <sys/timerfd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

namespace Xgeer {
namespace Detail {

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if (timerfd < 0) {
        fprintf(stderr, "Failed in timerfd_create");
        abort();
    }

    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();

    if (microseconds < 100) {
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / (1000 * 1000));
    ts.tv_nsec = static_cast<long>(microseconds % (1000 * 1000) * 1000);

    return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;

    // read
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));

    if (n != sizeof(howmany)) {
        fprintf(stderr, "TimerQueue::handleRead() reads %ld bytes instead of 8", n);
    }
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
    struct itimerspec newvalue;
    struct itimerspec oldvalue;

    bzero(&newvalue, sizeof(newvalue));
    bzero(&oldvalue, sizeof(oldvalue));

    newvalue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newvalue, &oldvalue);
    if (ret) {
        fprintf(stderr, "Failed in timerfd_settime()");
    }
}

}
}
