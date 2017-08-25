all: test1 test2 test3 test4
	rm *.o

test1: test1.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o
	g++ -o test1 test1.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o -lpthread
test2: test2.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o
	g++ -o test2 test2.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o -lpthread
test3: test3.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o
	g++ -o test3 test3.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o -lpthread
test4: test4.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o
	g++ -o test4 test4.o Thread.o EventLoop.o Poller.o TimerQueue.o Channel.o Timestamp.o Timer.o -lpthread

test1.o: test1.cpp
	g++ -c -std=c++11 test1.cpp
test2.o: test2.cpp
	g++ -c -std=c++11 test2.cpp
test3.o: test3.cpp
	g++ -c -std=c++11 test3.cpp
test4.o: test4.cpp
	g++ -c -std=c++11 test4.cpp

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
