//
// ThreadPool.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "ThreadPool.h"
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <exception>

using namespace Dalin;

ThreadPool::ThreadPool(const std::string &name)
 : mutex_(),
   notEmpty_(mutex_),
   notFull_(mutex_),
   name_(name),
   maxQueueSize_(0),
   running_(false)
{
}

ThreadPool::~ThreadPool()
{
    if (running_) {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);

    for (int i = 0; i != numThreads; ++i) {
        char id[32];
        snprintf(id, sizeof(id), "%d", i + 1);
        threads_.push_back(std::unique_ptr<Thread>(new Thread([&](){ this->runInThread(); }, name_ + id)));
        threads_[i]->start();
    }

    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
}

void ThreadPool::stop()
{
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
        notEmpty_.notifyAll();
    }

    std::for_each(threads_.begin(), threads_.end(), [](std::unique_ptr<Thread> &thread){ thread->join(); });
}

size_t ThreadPool::queueSize() const
{
    MutexLockGuard lock(mutex_);
    return taskQueue_.size();
}

void ThreadPool::addTask(const Task &task)
{
    if (threads_.empty()) {
        task();
    }
    else {
        MutexLockGuard lock(mutex_);
        while (isFull()) {
            notFull_.wait();
        }
        assert(!isFull());

        taskQueue_.push_back(task);
        notEmpty_.notify();
    }
}

bool ThreadPool::isFull() const
{
    mutex_.assertLocked();
    return maxQueueSize_ > 0 && taskQueue_.size() >= maxQueueSize_;
}

ThreadPool::Task ThreadPool::takeTask()
{
    MutexLockGuard lock(mutex_);
    while (taskQueue_.empty() && running_) {
        notEmpty_.wait();
    }

    Task task;
    if (!taskQueue_.empty()) {
        task = taskQueue_.front();
        taskQueue_.pop_front();

        if (maxQueueSize_ > 0) {
            notFull_.notify();
        }
    }

    return task;
}

void ThreadPool::runInThread()
{
    try {
        if (threadInitCallback_) {
            threadInitCallback_();
        }
        while (running_) {
            Task task(takeTask());
            if (task) {
                task();
            }
        }
    }
    catch (std::exception &ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...) {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw;
    }
}
