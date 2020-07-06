cmake_minimum_required(VERSION 3.6)
project(hierarchical_timer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    LinkedListApi.c
    LinkedListApi.h
    main.c
    WheelTimer.c
    WheelTimer.h)

add_executable(hierarchical_timer ${SOURCE_FILES})