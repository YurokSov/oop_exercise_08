cmake_minimum_required(VERSION 3.16)
project(lab8)

set(CMAKE_CXX_FLAGS "-Wall -g -pthread")

add_executable(oop_exercise_08 main.cpp figure.cpp triangle.cpp square.cpp rectangle.cpp)