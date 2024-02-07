add_library(argparser STATIC ArgParser.cpp
        ArgParser.h
        ArgumentBuilder.h
        Argument.h
        ConcreteArgumentBuilder.hpp
        ConcreteArgument.cpp
        ConcreteArgument.hpp
)

add_subdirectory(basic)

target_link_libraries(argparser PUBLIC argparser_basic)

target_include_directories(argparser PUBLIC ${PROJECT_SOURCE_DIR})
