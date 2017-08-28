//
// Poller.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef POLLER_H
#define POLLER_H

#include <map>
#include <vector>

#include "../base/Timestamp.h"
#include "EventLoop.h"

struct pollfd;

namespace Dalin {
namespace Net {

class Channel;

// IO multiplexing with poll.
// This class doesn't own the Channel objects.
class Poller : Noncopyable {
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop *loop);
    ~Poller();

    // Poll the I/O events.
    // Must be called in the loop thread.
    Timestamp poll(int timeouts, ChannelList *activeChannels);

    // Changes the interested I/O events.
    // Must be called in the loop thread.
    void updateChannel(Channel *channel);

    // Remove the channel, when it destructs.
    // Must be called in the loop thread.
    void removeChannel(Channel *channel);

    void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

private:
    void fillActiveChannels(int numEvents, ChannelList *activeChannels);

    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel*> ChannelMap;

    EventLoop *ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
};


}
}

#endif
