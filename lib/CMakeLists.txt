add_library(argparser ArgParser.cpp
        ArgParser.h
        ArgumentBuilder.cpp
        ArgumentBuilder.h
        Argument.cpp
        Argument.h
        BoolArgument.cpp
        BoolArgument.hpp
        CompositeArgument.cpp
        CompositeArgument.hpp
        LongArgument.cpp
        LongArgument.hpp
        StringArgument.cpp
        StringArgument.hpp
        HelpComposer.cpp
        HelpComposer.h
        BasicFunctions.cpp
        BasicFunctions.hpp
        BoolArgumentBuilder.cpp
        BoolArgumentBuilder.h
        CompositeArgumentBuilder.cpp
        CompositeArgumentBuilder.h
        LongArgumentBuilder.cpp
        LongArgumentBuilder.h
        StringArgumentBuilder.cpp
        StringArgumentBuilder.h
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")