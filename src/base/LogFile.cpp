//
// LogFile.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "LogFile.h"
#include <unistd.h>
#include <assert.h>
#include "FileUtil.h"

namespace Xgeer{
namespace Detail {
    std::string hostname()
    {
        char buf[256];
        if (gethostname(buf, sizeof(buf)) == 0) {
            buf[sizeof(buf) - 1] = '\0';
            return buf;
        }
        else {
            return "unknownhost";
        }
    }
}

LogFile::LogFile(const std::string &baseName, size_t rollSize)
 : baseName_(baseName),
   rollSize_(rollSize)
{
    assert(baseName_.find('/') == std::string::npos);
    rollFile();
}

void LogFile::flush()
{
    file_->flush();
}

void LogFile::rollFile()
{
    time_t now = 0;
    std::string fileName = getLogFileName(baseName_, &now);

    file_.reset(new FileUtil::AppendFile(fileName));
}

void LogFile::append(const char *logLine, size_t len)
{
    file_->append(logLine, len);
    if (file_->wrritenBytes() > rollSize_) {
        rollFile();
    }
}

std::string LogFile::getLogFileName(const std::string &baseName, time_t *now)
{
    std::string fileName;
    fileName.reserve(baseName.size() + 64);
    fileName = baseName;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    fileName += timebuf;

    fileName += Detail::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), "%.d", getpid());
    fileName += pidbuf;

    fileName += ".log";

    return fileName;
}
}
