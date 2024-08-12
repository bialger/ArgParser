# Библиотека ArgParser

Данная библиотека определяет POSIX-совместимый парсер аргументов командной строки - библиотеку `argparser`.
Использует CMake.
Она была написана как лабораторная работа в курсе "Основы программирования на C++" в программе ITMO SE.
С заданием можно ознакомиться [здесь](./TASK.md).

Документация библиотеки находится [во вложенной папке](./lib/argparser/docs/README.md).

## Использование в проекте

Данную библиотеку можно получить через FetchContent:

```cmake
FetchContent_Declare(argparser GIT_REPOSITORY https://github.com/bialger/ArgParser)
FetchContent_MakeAvailable(argparser)
```
