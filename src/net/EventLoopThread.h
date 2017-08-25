//
// EventLoopThread.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Noncopyable.h"

namespace Dalin {
namespace Net {

class EventLoop;

class EventLoopThread : Noncopyable {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *startLoop();

private:
    void threadFunc();

    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

}
}

#endif
