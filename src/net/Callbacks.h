//
// Callbacks.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <functional>
#include <memory>

#include "../base/Timestamp.h"

namespace Dalin {
namespace Net {

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void ()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&, const char *data, ssize_t len)> MessageCallback;

}
}

#endif
