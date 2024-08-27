# Библиотека ArgParser

Данная библиотека определяет POSIX-совместимый парсер аргументов командной строки - библиотеку `argparser`.
Использует CMake.
Она была написана как лабораторная работа в курсе "Основы программирования на C++" в программе ITMO SE.
С заданием можно ознакомиться [здесь](./TASK.md).

В библиотеке определен парсинг стандартных примитивов, таких как `int`, `float`, `char` и т.д., а также
парсинг `std::string`.
Есть возможность добавлять логику парсинга для иных типов документов (см. [пример](./bin/main.cpp)).

Документация библиотеки находится [во вложенной папке](./lib/argparser/docs/README.md).

## Использование в проекте

Более подробно об использовании ArgParser можно прочитать на странице 
[пользовательской документации](./lib/argparser/docs/ArgParser.md).

### CMake

Данную библиотеку можно получить через FetchContent:

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser)
FetchContent_MakeAvailable(argparser)

target_link_libraries(MyTarget PUBLIC argparser)
```

Можно использовать конкретную ветку, тег версии или коммит:

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser GIT_TAG dev)
```

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser GIT_TAG v1.1.0)
```

## Пример использования и добавления аргумента

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
```

main.cpp:

```cpp
#include <iostream>
#include <argparser/ArgParser.hpp>

enum class Action {
  kNone = 0,
  kSum = 1,
  kMul = 2,
};

std::ostream& operator<<(std::ostream& os, Action action) {
  switch (action) {
    case Action::kSum:
      os << "sum";
      break;
    case Action::kMul:
      os << "mul";
      break;
    case Action::kNone:
      os << "none";
      break;
  }

  return os;
}

ArgumentParser::NonMemberParsingResult<Action> ParseAction(const std::string& action) {
  ArgumentParser::NonMemberParsingResult<Action> result{};

  if (action == "sum") {
    result.value = Action::kSum;
  } else if (action == "mul") {
    result.value = Action::kMul;
  } else {
    result.success = false;
    result.value = Action::kNone;
  }

  return result;
}

AddArgumentType(Action, ParseAction)

void InitializeParser(ArgumentParser::ArgParser& parser) {
  std::string action_alias = "Action";
  std::string action_description = "Action type";
  parser.SetAliasForType<Action>(action_alias);
  parser.AddArgument<Action>('a', "action", action_description);
  parser.AddHelp('h', "help", "Program accumulate arguments");
}

int main(int argc, char *argv[]) {
  ArgumentParser::ArgParser parser("TestArgParser", PassArgumentTypes(Action));
  InitializeParser(parser);
  bool result = parser.Parse(argc, argv, {std::cerr, true});

  if (!result) {
    std::cout << parser.HelpDescription() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.HelpDescription() << std::endl;
    return 0;
  }
  
  if (parser.GetValue<Action>() == Action::kSum) {
    std::cout << "Summarize" << std::endl;
  } else if (parser.GetValue<Action>() == Action::kMul) {
    std::cout << "Multiply" << std::endl;
  }

  std::cout << std::boolalpha << parser.GetFlag("test") << std::endl;
  return 0;
}
```
