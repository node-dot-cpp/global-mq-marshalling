#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_library(global-mq
    src/marshalling.cpp
    src/gmqueue.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    # TODO fix code and remove
    target_compile_options(global-mq PUBLIC -Wno-ambiguous-reversed-operator)
endif()


target_include_directories(global-mq PUBLIC include)
target_link_libraries(global-mq fmt::fmt)
