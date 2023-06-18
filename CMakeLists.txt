cmake_minimum_required(VERSION 3.25)
project(Multithreaded_PseudoRandGenerator C)

set(CMAKE_C_STANDARD 23)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(Multithreaded_PseudoRandGenerator multithreaded-PseudoRandGenerator.c)
#add_executable(Multithreaded_PseudoRandGenerator Predictor.c Predictor.h)
target_link_libraries(Multithreaded_PseudoRandGenerator PRIVATE Threads::Threads)
