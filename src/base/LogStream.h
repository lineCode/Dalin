//
// LogStream.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include "Noncopyable.h"
#include <string>

namespace Xgeer {
    class LogStream : Noncopyable {
    public:
        typedef LogStream self;

        template <typename T>
        self &operator<<(T v)
        {
            buffer_.append(std::to_string(v));
            return *this;
        }

        self &operator<<(bool v)
        {
            buffer_.append(v ? "1" : "0");
            return *this;
        }

        self &operator<<(char v)
        {
            char str[2] = {v, 0};
            buffer_.append(str);
            return *this;
        }

        self &operator<<(const char *str)
        {
            if (str) {
                buffer_.append(str);
            }
            else {
                buffer_.append("(null)");
            }
            return *this;
        }
        self &operator<<(const unsigned char *str)
        {
            *this << reinterpret_cast<const char *>(str);
            return *this;
        }

        self &operator<<(const std::string &v)
        {
            buffer_.append(v);
            return *this;
        }

        void append(const char *data) { buffer_.append(data); }
        const std::string &buffer() const { return buffer_; }
        void resetBufffer() { buffer_.clear(); }

    private:
        std::string buffer_;
    };
}

#endif
