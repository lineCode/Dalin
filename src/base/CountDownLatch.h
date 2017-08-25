//
// CountDownLatch.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"
#include "Noncopyable.h"

namespace Dalin {

class CountDownLatch : Noncopyable {
public:
    explicit CountDownLatch(int count);

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
