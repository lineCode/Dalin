//
// EventLoop.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "../base/Thread.h"
#include "../base/Noncopyable.h"
#include "../base/CurrentThread.h"

namespace Xgeer {
namespace Net {

class EventLoop : Noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void assertInLoopThread()
    {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return tid_ == CurrentThread::tid(); }
private:
    void abortNotInLoopThread();

    bool looping_;
    const pid_t tid_;
};

}
}

#endif
