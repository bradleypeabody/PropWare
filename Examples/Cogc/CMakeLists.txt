cmake_minimum_required(VERSION 3.3)
find_package(PropWare 2.0.0 REQUIRED)

project(Cogc_Demo CXX COGCXX)

create_simple_executable(${PROJECT_NAME}
    Cogc_Demo.cpp
    blinky.cogcpp)
