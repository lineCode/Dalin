//
// Timestamp.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Timestamp.h"
#include <inttypes.h>
#include <sys/time.h>

using namespace Xgeer;

std::string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / (1000 * 1000);
    int64_t microseconds = microSecondsSinceEpoch_ % (1000 * 1000);
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64, seconds, microseconds);
    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * (1000 * 1000) + tv.tv_usec);
}
