#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_executable(generator
    idl/global_mq_IDL.tab.cpp
    idl/lex.yy.cpp
    src/global_mq_parser_helper.cpp
    src/idl_tree_serializer.cpp
    src/main.cpp
)

target_include_directories(generator PUBLIC include)
target_link_libraries(generator fmt::fmt)
