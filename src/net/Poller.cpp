//
// Poller.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Poller.h"
#include "Channel.h"

#include <assert.h>
#include <poll.h>

using namespace Dalin;
using namespace Dalin::Net;

Poller::Poller(EventLoop *loop)
 : ownerLoop_(loop)
{

}

Poller::~Poller()
{

}

Timestamp Poller::poll(int timeouts, ChannelList *activeChannels)
{
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeouts);
    Timestamp now(Timestamp::now());

    if (numEvents > 0) {
        fillActiveChannels(numEvents, activeChannels);
    }
    else if (numEvents < 0) {
        fprintf(stderr, "Failed in Poller::poll()\n");
    }

    return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList *activeChannels)
{
    for (auto pfd = pollfds_.begin(); pfd != pollfds_.end(); ++pfd) {
        if (pfd->revents > 0) {
            --numEvents;

            auto ch =channels_.find(pfd->fd);
            assert(ch != channels_.end());

            Channel *channel = ch->second;
            assert(channel->fd() == pfd->fd);
            channel->set_revents(pfd->revents);

            activeChannels->push_back(channel);
        }
    }
}

void Poller::updateChannel(Channel *channel)
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
        // update existing one
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] == channel);

        int idx = channel->index();
        assert(0 <= idx && idx <static_cast<int>(pollfds_.size()));
        struct pollfd &pfd = pollfds_[idx];
        assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() -1);
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if (channel->isNoneEvent()) {
            // ignore this pollfd
            pfd.fd = -channel->fd() - 1;
        }
    }
}

void Poller::removeChannel(Dalin::Net::Channel *channel)
{
    assertInLoopThread();
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    assert(channel->isNoneEvent());

    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));

    const struct pollfd &pfd = pollfds_[idx];
    assert((pfd.fd == -channel->fd() - 1) && pfd.events == channel->events());

    size_t n = channels_.erase(channel->fd());
    assert(n == 1);

    if (static_cast<size_t>(idx) == pollfds_.size() - 1) {
        pollfds_.pop_back();
    }
    else {
        int channelAtEnd = pollfds_.back().fd;
        std::iter_swap(pollfds_.begin() + idx, pollfds_.end() - 1);
        if (channelAtEnd < 0) {
            channelAtEnd = -channelAtEnd - 1;
        }
        channels_[channelAtEnd]->setIndex(idx);

        pollfds_.pop_back();
    }
}
