//
// Mutex.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "Thread.h"
#include "CurrentThread.h"
#include "Noncopyable.h"

#define MCHECK(ret) ({ __typeof__(ret) errnum = (ret); \
                       assert(errnum == 0); })

namespace Dalin {

class MutexLock : Noncopyable {
public:
    MutexLock()
     : holder_(0)
    {
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock()
    {
        assert(holder_ == 0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    bool isLockedByThisThread() const
    {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        MCHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex_));
    }

    pthread_mutex_t *getPthreadMutex()
    {
        return &mutex_;
    }

private:
    friend class Condition;

    class UnassignGuard : Noncopyable {
    public:
        UnassignGuard(MutexLock &mutex)
         : owner_(mutex)
        {
            owner_.unassignHolder();
        }
        ~UnassignGuard()
        {
            owner_.assignHolder();
        }

    private:
        MutexLock &owner_;
    };

    void unassignHolder()
    {
        holder_ = 0;
    }

    void assignHolder()
    {
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

class MutexLockGuard : Noncopyable {
public:
    MutexLockGuard(MutexLock &mutex)
     : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};

}

#endif
