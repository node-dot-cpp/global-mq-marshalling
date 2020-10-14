#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project(global-mq CXX)

include(CTest)

add_subdirectory(3rdparty/fmt)

add_executable(generator
    generator/idl/global_mq_IDL.tab.cpp
    generator/idl/lex.yy.cpp
    generator/src/global_mq_parser_helper.cpp
    generator/src/idl_tree_serializer.cpp
    generator/src/main.cpp
)

target_include_directories(generator PUBLIC generator/include)
target_link_libraries(generator fmt::fmt)
