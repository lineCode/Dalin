//
// EventLoopThreadPool.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"

#include <vector>
#include <memory>
#include <functional>

namespace Dalin {
namespace Net {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : Noncopyable {
public:
    EventLoopThreadPool(EventLoop *baseLoop);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads)
    {
        numThreads_ = numThreads;
    }

    void start();

    EventLoop *getNextLoop();

private:
    EventLoop *baseLoop_;
    bool started_;
    int numThreads_;
    int next_; // always in the loop thread
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};

}
}

#endif
