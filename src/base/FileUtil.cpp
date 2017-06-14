//
// FileUtil.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "FileUtil.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

using namespace Xgeer::FileUtil;

AppendFile::AppendFile(std::string fileName)
: fp_(fopen(fileName.c_str(), "ae")),
  writtenBytes_(0)
{
    assert(fp_);
    setbuffer(fp_, buf_, sizeof(buf_));
}

AppendFile::~AppendFile()
{
    fclose(fp_);
}

void AppendFile::append(const char *logLine, size_t len)
{
    size_t n = write(logLine, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = write(logLine + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
            }
            break;
        }
        n += x;
        remain = len - n;
    }

    writtenBytes_ += n;
}

void AppendFile::flush()
{
    fflush(fp_);
}

size_t AppendFile::write(const char *logLine, size_t len)
{
    return fwrite_unlocked(logLine, 1, len, fp_);
}
