cmake_minimum_required(VERSION 3.10)

set(SOURCES main.c)
find_package(Threads)

add_executable(catdog ${SOURCES})

target_link_libraries(catdog ${CMAKE_THREAD_LIBS_INIT})