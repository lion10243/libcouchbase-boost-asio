cmake_minimum_required(VERSION 3.0)
project(lcb-boost-asio CXX C)

find_package(Boost 1.54 REQUIRED)

add_library(lcb-boost-plugin lcb-boost-plugin.cpp)
target_link_libraries(lcb-boost-plugin boost-system)
target_compile_options(lcb-boost-plugin BEFORE PUBLIC -std=c++11 -Wall -Wextra -g)
