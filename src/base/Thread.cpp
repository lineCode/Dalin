//
// Thread.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "CurrentThread.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace Laardi {
namespace CurrentThread {
    __thread pid_t t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "unknown";

    void cacheTid()
    {
        if (t_cachedTid == 0) {
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }

    bool isMainThread()
    {
        return tid() == getpid();
    }

    void sleepUsec(int64_t usec)
    {
        struct timespec ts = {0, 0};
        ts.tv_sec = static_cast<time_t>(usec / (1000 * 1000));
        ts.tv_nsec = static_cast<long>(usec % (1000 * 1000) * 1000);
        ::nanosleep(&ts, NULL);
    }
}
}
