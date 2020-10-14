#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(FLEX_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/idl/lex.yy.cpp)
set(BISON_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/idl/global_mq_IDL.tab.cpp)

if (BISON_FOUND AND FLEX_FOUND)
    add_custom_command(
            OUTPUT ${FLEX_OUTPUT} ${BISON_OUTPUT}
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/idl/global_mq_grammar.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/idl
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/idl/global_mq_IDL.l ${CMAKE_CURRENT_SOURCE_DIR}/idl/global_mq_IDL.y 
            COMMENT "Generating flex/bison"
    )
endif ()


add_executable(generator
    ${FLEX_OUTPUT}
    ${BISON_OUTPUT}
    src/global_mq_parser_helper.cpp
    src/idl_tree_serializer.cpp
    src/main.cpp
)

target_include_directories(generator PUBLIC include)
target_link_libraries(generator fmt::fmt)
