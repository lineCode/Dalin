//
// EventLoop.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "../base/Timestamp.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Noncopyable.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <memory>
#include <vector>

namespace Xgeer {
namespace Net {

class Channel;
class Poller;
class TimerQueue;

class EventLoop : Noncopyable {
private:
    void abortNotInThread();
    void handleRead();
    void doPendingFunctors();

    //typedef std::vector<> TODO
};

}
}

#endif
