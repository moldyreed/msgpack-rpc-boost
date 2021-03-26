project(mprpc)
cmake_minimum_required(VERSION 2.8)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
set(BUILD_TESTS $ENV{BUILD_TESTS})

##########################################
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

option(shared "build msgpack-rpc-boost as a shared library" OFF)

if(shared)
    add_definitions(-DBOOST_ALL_DYN_LINK)
else()
    set(Boost_USE_STATIC_LIBS ON)
    add_definitions(-DBOOST_ASIO_SEPARATE_COMPILATION)
endif()

find_package (Boost REQUIRED COMPONENTS system thread filesystem log)

if(NOT shared)
    set( _messagepack_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

find_package (msgpack REQUIRED)

if(NOT shared)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${_messagepack_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

include_directories (
    ${Boost_INCLUDE_DIRS}
    ${msgpack_INCLUDE_DIR})

link_libraries (
    ${Boost_LIBRARIES}
    msgpack)

##########################################

ADD_SUBDIRECTORY(src/msgpack/rpc)

if(BUILD_TESTS)
MESSAGE("-- Build test applications")
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(unittest)
endif(BUILD_TESTS)
