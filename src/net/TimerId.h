//
// TimerId.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TIMERID_H
#define TIMERID_H

namespace Dalin {
namespace Net {

class Timer;

class TimerId {
public:
    explicit TimerId(Timer *timer)
     : value_(timer)
    {

    }

private:
    Timer *value_;
};

}
}

#endif
