include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        argparser_tests
        argparser_unit_tests.cpp
        test_functions.cpp
        test_functions.hpp
        ArgParserUnitTestSuite.cpp
        ArgParserUnitTestSuite.hpp
)

target_link_libraries(argparser_tests PUBLIC
        argparser
        GTest::gtest_main
        PRIVATE argparser_basic
)

target_include_directories(argparser_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(argparser_tests)
