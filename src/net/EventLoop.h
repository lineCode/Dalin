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
#include <vector>
#include <memory>

namespace Xgeer {
namespace Net {

class Channel;
class Poller;

class EventLoop : Noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void quit();

    // internal use only
    void updateChannel(Channel *channel);

    void assertInLoopThread()
    {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return tid_ == CurrentThread::tid(); }
private:
    void abortNotInLoopThread();

    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    bool quit_;
    const pid_t tid_;
    std::unique_ptr<Poller> poller_;
    ChannelList activeChannels_;
};

}
}

#endif
