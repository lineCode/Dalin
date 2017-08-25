//
// CurrentThread.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <pthread.h>
#include <stdint.h>

namespace Dalin {
namespace CurrentThread {

extern __thread pid_t t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char *t_threadName;

void cacheTid();

inline pid_t tid()
{
    if (t_cachedTid == 0) {
        cacheTid();
    }
    return t_cachedTid;
}

inline const char *tidString()
{
    return t_tidString;
}

inline int tidStringLength()
{
    return t_tidStringLength;
}

inline const char *threadName()
{
    return t_threadName;
}

bool isMainThread();

void sleepUsec(int64_t usec);

}
}

#endif
