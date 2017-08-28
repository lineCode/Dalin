//
// Buffer.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Buffer.h"
#include "SocketsOps.h"

#include <errno.h>
#include <memory.h>
#include <sys/uio.h>

using namespace Dalin::Net;

ssize_t Buffer::readFd(int fd, int *savedErrno)
{
    char extraBuf[65536];
    struct iovec vec[2];

    const size_t writable = writableBytes();

    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof(extraBuf);

    const ssize_t n = readv(fd, vec, 2);
    if (n < 0) {
        *savedErrno = errno;
    }
    else if (static_cast<size_t>(n) <= writable) {
        writerIndex_ += n;
    }
    else {
        writerIndex_ = buffer_.size();
        append(extraBuf, n - writable);
    }

    return n;
}
