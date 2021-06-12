cmake_minimum_required(VERSION 3.10)

project(ZMQ&HTTP)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_executable(ZMQHTTPServer ZMQ&HTTPServer.cpp httplib.h)
add_executable(ZMQHTTPClient ZMQ&HTTPClient.cpp httplib.h)

#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
target_link_libraries(ZMQHTTPServer cppzmq)
target_link_libraries(ZMQHTTPClient cppzmq)