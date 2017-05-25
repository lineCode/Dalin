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

#define MCHECK(ret) ({ __typeof__(ret) errnum = (ret);  \
                       assert(errnum == 0); })

namespace Xgeer {
    class MutexLock {
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

        MutexLock(const MutexLock&) = delete;
        MutexLock &operator=(const MutexLock&) = delete;

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

        class UnassignGuard {
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

            UnassignGuard(const UnassignGuard&) = delete;
            UnassignGuard &oprator(const UnassignGuard&) = delete;

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

    class MutexLockGuard {
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

        MutexLockGuard(const MutexLockGuard&) = delete;
        MutexLockGuard &operator=(const MutexLockGuard&) = delete;


    private:
        MutexLock &mutex_;
    };
}

#endif
