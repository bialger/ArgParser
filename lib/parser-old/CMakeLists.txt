add_library(lib_parser STATIC
        arguments_parser.cpp
        arguments_parser.hpp
        argument.hpp
        bool_argument.cpp
        bool_argument.hpp
        composite_argument.cpp
        composite_argument.hpp
        long_argument.cpp
        long_argument.hpp
        string_argument.cpp
        string_argument.hpp
)

target_link_libraries(lib_parser PUBLIC lib_basic)

target_include_directories(lib_parser PUBLIC ${PROJECT_SOURCE_DIR})
