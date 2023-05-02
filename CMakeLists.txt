cmake_minimum_required(VERSION 3.8)

project(chat)

set(CMAKE_CXX_STANDART 17)

message("CXX Standart: ${CMAKE_CXX_STANDART}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -Wall -O3")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(SOURCES_FILES src/main.cpp src/chat.cpp src/sha1.cpp)

add_executable(chat ${SOURCES_FILES})