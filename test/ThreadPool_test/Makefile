ThreadPool_test: ThreadPool_test.o CountDownLatch.o Thread.o ThreadPool.o
	g++ -o ThreadPool_test ThreadPool_test.o CountDownLatch.o Thread.o ThreadPool.o -lpthread
	rm *.o
ThreadPool_test.o: ThreadPool_test.cpp
	g++ -c -std=c++11 ThreadPool_test.cpp
CountDownLatch.o: ../../src/base/CountDownLatch.cpp
	g++ -c -std=c++11 ../../src/base/CountDownLatch.cpp
Thread.o: ../../src/base/Thread.cpp
	g++ -c -std=c++11 ../../src/base/Thread.cpp
ThreadPool.o: ../../src/base/ThreadPool.cpp
	g++ -c -std=c++11 ../../src/base/ThreadPool.cpp
