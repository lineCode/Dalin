//
// Channel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include "../base/Noncopyable.h"

namespace Xgeer {
namespace Net {

class EventLoop;

// A selectable I/O channel.
// This class doesn't own the file descriptor.
// The file descriptor could be a socket, an eventfd, a timerfd, or a signalfd.
class Channel {
public:
    typedef std::function<void()> EventCallback;

    Channel(EventLoop *loop, int fd);

    void handleEvent();

    void setReadCallback(const EventCallback &cb)
    {
        readCallback_ = cb;
    }
    void setWriteCallback(const EventCallback &cb)
    {
        writeCallback_ = cb;
    }
    void setErrorCallback(const EventCallback &cb)
    {
        errorCallback_ = cb;
    }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void setRevents(int revt) { revents_ = revt; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }

    int index() const { return index_; }
    void setIndex(int idx) { index_ = idx; }

    EventLoop *ownerLoop() const { return loop_; }
private:
    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};

}
}

#endif
