//
// ThreadPool.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include "CurrentThread.h"
#include "Noncopyable.h"
#include <pthread.h>
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <deque>

namespace Dalin {

class ThreadPool : Noncopyable {
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const std::string &name = std::string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize)
    {
        maxQueueSize_ = maxSize;
    }

    void setThreadInitCallback(const Task &cb)
    {
        threadInitCallback_ = cb;
    }

    void start(int numThreads);
    void stop();

    const std::string name() const
    {
        return name_;
    }

    size_t queueSize() const;
    void addTask(const Task &task);

private:
    bool isFull() const;
    void runInThread();
    Task takeTask();

    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    std::string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<Task> taskQueue_;
    size_t maxQueueSize_;
    bool running_;
};

}

#endif
