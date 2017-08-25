//
// Thread.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <string>
#include <functional>
#include "Noncopyable.h"

namespace Dalin {

class Thread : Noncopyable {
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(const ThreadFunc&, const std::string &name = std::string());
    ~Thread();

    void start();
    int join();

    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string &name() const { return name_; }

    // Only called by void* Detail::startThread(void *arg)
    void run();
private:
    void setDefaultName();

    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
};

}

#endif
