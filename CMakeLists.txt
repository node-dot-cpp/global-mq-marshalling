#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-2021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project(global-mq LANGUAGES CXX)
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

# by default only when not a subproject
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(GLOBAL_MQ_TEST ON CACHE BOOL "Enable global-mq tests")
else()
  set(GLOBAL_MQ_TEST OFF CACHE BOOL "Enable global-mq tests")
endif()


if(GLOBAL_MQ_TEST)
  add_subdirectory(test/cpp)
  add_subdirectory(test/cs)
  add_subdirectory(test/csharp-2-cpp)
endif()
