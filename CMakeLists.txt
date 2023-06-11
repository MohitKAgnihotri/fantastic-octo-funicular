cmake_minimum_required(VERSION 3.25)
project(Multithreaded C)

set(CMAKE_C_STANDARD 23)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(Multithreaded main.c)
target_link_libraries(Multithreaded PRIVATE Threads::Threads)

