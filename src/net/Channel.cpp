//
// Channel.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Channel.h"
#include "EventLoop.h"

#include <poll.h>
#include <stdio.h>

using namespace Dalin::Net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
 : loop_(loop),
   fd_(fd),
   events_(0),
   revents_(0),
   index_(-1),
   eventHandling_(false)
{

}

Channel::~Channel()
{
    assert(!eventHandling_);
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    eventHandling_ = true;

    if (revents_ & POLLNVAL) {
        fprintf(stdout, "Channel::handleEvent() POLLNVAL");
    }

    if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        if (CloseCallback_) {
            CloseCallback_();
        }
    }

    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) {
            readCallback_(receiveTime);
        }
    }
    if (revents_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }

    eventHandling_ = false;
}
