#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-1021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------


if(MSVC)
    add_test(NAME test-csharp
        COMMAND dotnet test /p:Configuration=Release /p:Platform=AnyCPU
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()