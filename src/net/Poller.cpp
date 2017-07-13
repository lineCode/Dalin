//
// Poller.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Poller.h"
#include "Channel.h"
#include <assert.h>
#include <poll.h>
#include <stdio.h>

using namespace Xgeer;
using namespace Xgeer::Net;

Poller::Poller(EventLoop *loop)
 : ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

Timestamp Poller::poll(int timeoutMs, ChannelList *activeChannels)
{
    // Shouldn't change pollfds_ in this function
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
    Timestamp now(Timestamp::now());

    if (numEvents > 0) {
        fillActiveChannels(numEvents, activeChannels);
    }
    else if (numEvents < 0) {
        fprintf(stderr, "Failed in Poller:poll()\n");
    }

    return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for (auto ptrFd = pollfds_.begin(); ptrFd != pollfds_.end() && numEvents > 0; ++ ptrFd) {
        if (ptrFd->revents > 0) {
            -- numEvents;

            auto ptrCh = channels_.find(ptrFd->fd);
            assert(ptrCh != channels_.end());

            Channel *channel = ptrCh->second;
            assert(channel->fd() == ptrFd->fd);

            channel->setRevents(ptrFd->revents);

            activeChannels->push_back(channel);
        }
    }
}

void Poller::updateChannel(Xgeer::Net::Channel *channel)
{
    assertInLoopThread();

    if (channel->index() < 0) {
        // a new one, add to pollfds_
        assert(channels_.find(channel->fd()) == channels_.end());

        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);

        int idx = static_cast<int>(pollfds_.size()) - 1;
        channel->setIndex(idx);

        channels_[pfd.fd] = channel;
    }
    else {
        // update an existing one
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] = channel);

        int idx = channel->index();
        assert(idx >= 0 && idx < static_cast<int>(pollfds_.size()));

        struct pollfd &pfd = pollfds_[idx];
        assert(pfd.fd == channel->fd() || pfd.fd == -1);
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;

        if (channel->isNoneEvent()) {
            // ignore this pollfd
            pfd.fd = -1;
        }
    }
}
