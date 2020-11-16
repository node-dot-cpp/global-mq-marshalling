#-------------------------------------------------------------------------------------------
# Copyright (c) 2020, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

add_library(global-mq
    src/marshalling.cpp
)

target_include_directories(global-mq PUBLIC include)
target_link_libraries(global-mq fmt::fmt)
