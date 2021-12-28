#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-2021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project(global-mq CXX)
set(CMAKE_CXX_STANDARD 20)
include(CTest)

if(NOT TARGET fmt)
    add_subdirectory(3rdparty/fmt)
endif()

add_subdirectory(idl_compiler)
add_subdirectory(library)


#-------------------------------------------------------------------------------------------
# Tests and samples
#-------------------------------------------------------------------------------------------
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(GLOBAL_MQ_TEST ON CACHE BOOL "Enable global-mq tests")
else()
  set(GLOBAL_MQ_TEST OFF CACHE BOOL "Enable global-mq tests")
endif()

if(GLOBAL_MQ_TEST)
  # only when not a subproject
  add_subdirectory(test/cpp)
endif()
