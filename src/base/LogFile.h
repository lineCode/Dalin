//
// LogFile.h
//
// Copyright (c) 2017 Jiawei Feng
//

//
// LogFile class is the backend of this simple log library.
// It's only used by one thread.
//

#ifndef LOGFILE_H
#define LOGFILE_H

#include "Noncopyable.h"
#include "FileUtil.h"
#include <string>
#include <memory>
#include <time.h>

namespace Xgeer {
    class LogFile : Noncopyable {
    public:
        LogFile(const std::string &baseName, size_t rollSize);

        ~LogFile() {}

        void flush();

        void rollFile();

        void append(const char *logLine, size_t len);
    private:
        static std::string getLogFileName(const std::string &baseName, time_t *now);

        const std::string baseName_;
        const size_t rollSize_;
        std::unique_ptr<FileUtil::AppendFile> file_;
    };
}

#endif
