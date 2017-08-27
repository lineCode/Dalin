test: test.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o Acceptor.o InetAddress.o SocketsOps.o Socket.o
	g++ -o test test.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o Acceptor.o InetAddress.o SocketsOps.o Socket.o -lpthread
	rm *.o

test.o: test.cpp
	g++ -c -std=c++11 test.cpp
Thread.o: ../../src/base/Thread.cpp
	g++ -c -std=c++11 ../../src/base/Thread.cpp
EventLoop.o: ../../src/net/EventLoop.cpp
	g++ -c -std=c++11 ../../src/net/EventLoop.cpp
Poller.o: ../../src/net/Poller.cpp
	g++ -c -std=c++11 ../../src/net/Poller.cpp
TimerQueue.o: ../../src/net/TimerQueue.cpp
	g++ -c -std=c++11 ../../src/net/TimerQueue.cpp
Channel.o: ../../src/net/Channel.cpp
	g++ -c -std=c++11 ../../src/net/Channel.cpp
Timestamp.o: ../../src/base/Timestamp.cpp
	g++ -c -std=c++11 ../../src/base/Timestamp.cpp
Timer.o: ../../src/net/Timer.cpp
	g++ -c -std=c++11 ../../src/net/Timer.cpp
Acceptor.o: ../../src/net/Acceptor.cpp
	g++ -c -std=c++11 ../../src/net/Acceptor.cpp
InetAddress.o: ../../src/net/InetAddress.cpp
	g++ -c -std=c++11 ../../src/net/InetAddress.cpp
SocketsOps.o: ../../src/net/SocketsOps.cpp
	g++ -c -std=c++11 ../../src/net/SocketsOps.cpp
Socket.o: ../../src/net/Socket.cpp
	g++ -c -std=c++11 ../../src/net/Socket.cpp
