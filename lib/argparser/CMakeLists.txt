add_library(argparser STATIC ArgParser.cpp
        ArgParser.hpp
        ArgumentBuilder.hpp
        Argument.hpp
        ConcreteArgumentBuilder.hpp
        ConcreteArgument.hpp
)

add_subdirectory(basic)

target_link_libraries(argparser PRIVATE argparser_basic)

target_include_directories(argparser PUBLIC ${PROJECT_SOURCE_DIR})

# This is needed for FetchContent usage, if built as standalone has no effect
target_include_directories(argparser PUBLIC "${argparser_SOURCE_DIR}/lib")
target_include_directories(argparser PUBLIC "${ArgParser_SOURCE_DIR}/lib")
