cmake_minimum_required(VERSION 3.20)

project(TcpIpNetworkProgramming LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message("Using MSVC")
  link_libraries("ws2_32.lib")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message("Using GCC")
endif()

add_subdirectory(chap05)
