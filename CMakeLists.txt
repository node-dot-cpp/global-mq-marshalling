#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project(global-mq CXX)
set(CMAKE_CXX_STANDARD 17)
include(CTest)

add_subdirectory(3rdparty/fmt)

add_subdirectory(idl_compiler)
add_subdirectory(library)
add_subdirectory(test)
