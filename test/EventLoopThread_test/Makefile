all: test
	rm *.o

test: test.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o EventLoopThread.o
	g++ -o test test.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o EventLoopThread.o -lpthread

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
EventLoopThread.o: ../../src/net/EventLoopThread.cpp
	g++ -c -std=c++11 ../../src/net/EventLoopThread.cpp
