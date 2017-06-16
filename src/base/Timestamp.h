//
// Timestamp.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <sys/types.h>
#include <algorithm>
#include <string>

namespace Xgeer {

class Timestamp {
friend bool operator<(Timestamp, Timestamp);
friend bool operator==(Timestamp, Timestamp);

public:
    Timestamp()
     : microSecondsSinceEpoch_(0)
    {
    }

    Timestamp(int64_t microSecondsSinceEpoch_Arg)
     : microSecondsSinceEpoch_(microSecondsSinceEpoch_Arg)
    {
    }

    void swap(Timestamp &that)
    {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    std::string toString() const;

    bool valid() const { return microSecondsSinceEpoch_ > 0; }

    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    time_t seconsSinceEpoch() const
    {
        return static_cast<time_t>(
            microSecondsSinceEpoch_ / (1000 * 1000));
    }

    static Timestamp now();
    static Timestamp invalid() { return Timestamp(); }


private:
    int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
}

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff / (1000 * 1000));
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t adds = static_cast<int64_t>(seconds * 1000 * 1000);
    return Timestamp(timestamp.microSecondsSinceEpoch() + adds);
}

}

#endif
