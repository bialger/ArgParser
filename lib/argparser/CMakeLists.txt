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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")