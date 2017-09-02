# Dalin

Dalin is a C++ non-blocking network library with these features:

  * Only runs on Linux
  * C++11 syntax and POSIX Threads
  * Non-blocking, multi-threads
  * Simple API
  * One loop per thread + ThreadPool

## Build and example

This network library uses C++11 syntax and POSIX Threads so if you compile with g++ on Linux you have to use the flags '-lpthread', '-std=c++11' like this:

    g++ -std=c++11 XXXXX.cpp YYYYY.cpp ZZZZZ.cpp -lpthread

If you want to run the test example:

    cd test/XXXXXX_test/
    make
    ./test

## Contribution

You are very welcome to contibute!
