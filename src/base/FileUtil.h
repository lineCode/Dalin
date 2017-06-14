//
// FileUtil.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>
#include <string>
#include "Noncopyable.h"

namespace Xgeer {
namespace FileUtil {
    class AppendFile : Noncopyable {
    public:
        explicit AppendFile(std::string fileName);
        ~AppendFile();

        void append(const char *logLine, size_t len);

        void flush();

        size_t wrritenBytes() { return writtenBytes_; }

    private:
        size_t write(const char *logLine, size_t len);

        FILE *fp_;
        char buf_[64 * 1024];
        size_t writtenBytes_;
    };
}
}

#endif
