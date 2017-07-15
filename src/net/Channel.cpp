//
// Channel.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Channel.h"
#include "EventLoop.h"
#include <stdio.h>
#include <poll.h>

using namespace Xgeer::Net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
 : loop_(loop),
   fd_(fd),
   events_(0),
   revents_(0),
   index_(-1)
{
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent()
{
    if (revents_ & POLLNVAL) {
        fprintf(stderr, "Channel::handleEvent() POLLNVAL\n");
    }

    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }

    if (revents_ & (POLLIN | POLLPRI | POLLHUP)) {
        if (readCallback_) {
            readCallback_();
        }
    }

    if (revents_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}
