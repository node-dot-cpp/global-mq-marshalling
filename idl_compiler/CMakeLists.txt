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
    src/main.cpp
)

target_include_directories(idl_compiler PRIVATE include)
target_link_libraries(idl_compiler fmt::fmt)
