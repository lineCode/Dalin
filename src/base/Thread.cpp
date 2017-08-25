//
// Thread.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Thread.h"
#include "CurrentThread.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/prctl.h>
#include <exception>

namespace Dalin {
namespace CurrentThread {

__thread pid_t t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char *t_threadName = "unknown";

void cacheTid()
{
    if (t_cachedTid == 0) {
        t_cachedTid = static_cast<int>(::syscall(SYS_gettid));
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    }
}

bool isMainThread()
{
    return tid() == getpid();
}

void sleepUsec(int64_t usec)
{
    struct timespec ts = {0, 0};
    int usecPersecond = 1000 * 1000;

    ts.tv_sec = static_cast<time_t>(usec / usecPersecond);
    ts.tv_nsec = static_cast<long>(usec % usecPersecond * 1000);

    ::nanosleep(&ts, NULL);
}

}

namespace Detail {  // Not visible to users.

void *startThread(void *arg)
{
    Thread *thread = reinterpret_cast<Thread *>(arg);
    thread->run();
    return NULL;
}

}

Thread::Thread(const ThreadFunc &func, const std::string &name)
 : started_(false),
   joined_(false),
   pthreadId_(0),
   tid_(0),
   func_(func),
   name_(name)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName()
{
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", CurrentThread::tid());
        name_ = buf;
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    if (pthread_create(&pthreadId_, NULL, Detail::startThread, this)) {
        started_ = false;
        fprintf(stderr, "Failed in pthread_create\n");;
        abort();
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;

    return pthread_join(pthreadId_, NULL);
}

void Thread::run()
{
    tid_ = CurrentThread::tid();

    CurrentThread::t_threadName = name_.empty() ? "DalinThread" : name_.c_str();
    ::prctl(PR_SET_NAME, CurrentThread::t_threadName);

    try {
        func_();
        CurrentThread::t_threadName = "finished";
    }
    catch (std::exception &ex) {
        CurrentThread::t_threadName = "crashed";
        fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...) {
        CurrentThread::t_threadName = "crashed";
        fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
        throw;
    }
}

}
