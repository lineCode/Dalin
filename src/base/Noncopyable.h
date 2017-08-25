//
// Noncopyable.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace Dalin {

class Noncopyable {
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};

}

#endif
