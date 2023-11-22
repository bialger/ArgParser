add_library(argparser ArgParser.cpp
        ArgParser.h
        ArgumentBuilder.h
        Argument.h
        ConcreteArgumentBuilder.hpp
        ConcreteArgument.cpp
        ConcreteArgument.hpp
        BasicFunctions.cpp
        BasicFunctions.hpp
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")