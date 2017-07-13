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
        fprintf(stderr, "Error: Poller:poll()\n");
    }

    return now;
}
