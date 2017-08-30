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

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void ()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&, Buffer *buf, Timestamp)> MessageCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;

}
}

#endif
