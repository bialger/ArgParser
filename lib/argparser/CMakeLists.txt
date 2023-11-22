add_library(argparser STATIC ArgParser.cpp
        ArgParser.h
        ArgumentBuilder.h
        Argument.h
        ConcreteArgumentBuilder.hpp
        ConcreteArgument.cpp
        ConcreteArgument.hpp
)

target_link_libraries(argparser PUBLIC basic)

target_include_directories(argparser PUBLIC ${PROJECT_SOURCE_DIR})
