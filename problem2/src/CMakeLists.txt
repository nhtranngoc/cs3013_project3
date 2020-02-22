cmake_minimum_required(VERSION 3.10)

set(SOURCES main.c)
find_package(Threads)

add_executable(gradstud ${SOURCES})

target_link_libraries(gradstud ${CMAKE_THREAD_LIBS_INIT})