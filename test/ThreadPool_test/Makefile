test: test.o CountDownLatch.o Thread.o ThreadPool.o
	g++ -o test test.o CountDownLatch.o Thread.o ThreadPool.o -lpthread
	rm *.o
test.o: test.cpp
	g++ -c -std=c++11 test.cpp
CountDownLatch.o: ../../src/base/CountDownLatch.cpp
	g++ -c -std=c++11 ../../src/base/CountDownLatch.cpp
Thread.o: ../../src/base/Thread.cpp
	g++ -c -std=c++11 ../../src/base/Thread.cpp
ThreadPool.o: ../../src/base/ThreadPool.cpp
	g++ -c -std=c++11 ../../src/base/ThreadPool.cpp
