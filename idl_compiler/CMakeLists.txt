#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-2021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_executable(idl_compiler
    grammar/generated/lex.yy.cpp
    grammar/generated/global_mq_IDL.tab.cpp
    src/global_mq_parser_helper.cpp
    src/idl_tree_serializer.cpp
    src/idl_tree_serializer_messages.cpp
    src/idl_tree_serializer_publishables.cpp
    src/idl_tree_serializer_publishables_print_tree.cpp
    src/idl_tree_2_csharp.cpp
    src/idl_tree_2_csharp_publishables.cpp
    src/main.cpp
)

target_include_directories(idl_compiler PRIVATE include)
target_link_libraries(idl_compiler fmt::fmt)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    
    target_compile_options(idl_compiler PRIVATE -Wno-register)
    target_compile_options(idl_compiler PRIVATE -Wno-format)

    #clang only
    target_compile_options(idl_compiler PRIVATE -Wno-potentially-evaluated-expression)
endif()
