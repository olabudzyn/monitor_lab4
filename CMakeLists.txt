cmake_minimum_required(VERSION 3.15)
project(monitor_lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(monitor_lab4 main.cpp monitor.h MyMonitor.h Functions.h)
target_link_libraries(monitor_lab4 pthread)
