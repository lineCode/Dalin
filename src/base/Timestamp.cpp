//
// Timestamp.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Timestamp.h"
#include <sys/time.h>

using namespace Dalin;

Timestamp Timestamp::now()
{
    struct timeval tv;
    ::gettimeofday(&tv, NULL);

    int64_t seconds = tv.tv_sec;

    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::toString() const
{
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  //snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  snprintf(buf, sizeof(buf) - 1, "%ld %.06ld", seconds, microseconds);
  return buf;
}
