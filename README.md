# Библиотека ArgParser

Данная библиотека определяет POSIX-совместимый парсер аргументов командной строки - библиотеку `argparser`.
Использует CMake.
Она была написана как лабораторная работа в курсе "Основы программирования на C++" в программе ITMO SE.
С заданием можно ознакомиться [здесь](./TASK.md).

Документация библиотеки находится [во вложенной папке](./lib/argparser/docs/README.md).

## Использование в проекте

Более подробно об использовании ArgParser можно прочитать на странице [пользовательской документации](./lib/argparser/docs/ArgParser.md).

### CMake

Данную библиотеку можно получить через FetchContent:

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser)
FetchContent_MakeAvailable(argparser)

target_include_directories(MyTarget PUBLIC "${argparser_SOURCE_DIR}/lib")
```

Можно использовать конкретную ветку разработки, тег версии или коммит:

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser GIT_TAG dev)
```

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser GIT_TAG v1.1.0)
```

### Пример:

CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.28)
project(TestArgParser)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser)
FetchContent_MakeAvailable(argparser)

add_executable(TestArgParser main.cpp)

target_link_libraries(TestArgParser PRIVATE argparser)

target_include_directories(TestArgParser PUBLIC "${argparser_SOURCE_DIR}/lib")
```

main.cpp:

```cpp
#include <iostream>
#include <argparser/ArgParser.hpp>

int main(int argc, char *argv[]) {
  ArgumentParser::ArgParser parser("TestArgParser");
  parser.AddHelp('h', "help", "This program is an ArgParser FetchContent example.");
  parser.AddFlag('t', "test", "test argument");
  bool result = parser.Parse(argc, argv, {std::cerr, true});

  if (!result) {
    std::cout << parser.HelpDescription() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.HelpDescription() << std::endl;
    return 0;
  }

  std::cout << std::boolalpha << parser.GetFlag("test") << std::endl;
  return 0;
}
```
