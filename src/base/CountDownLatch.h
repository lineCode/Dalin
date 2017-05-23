//
// CountDownLatch.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace Laardi {
    class CountDownLatch {
    public:
        explicit CountDownLatch(int count);

        CountDownLatch(const CountDownLatch&) = delete;
        CountDownLatch &operator=(const CountDownLatch&) = delete;

        void wait();

        void countDown();

        int getCount() const;

    private:
        mutable MutexLock mutex_;
        Condition cond_;
        int count_;
    };
}

#endif
