[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12790326&assignment_repo_type=AssignmentRepo)
# Лабораторная работа 4

## Задача

Спроектировать и реализовать класс для парсинга аргументов командной строки.

## Требования

В качестве требований Вам дается файл с [тестами](tests/argparser_test.cpp). Исходя из тестов, Вы должны понять, какой публичный интерфейс предоставляет парсер. Ваша задача добиться, чтобы все тесты "проходили".

Изначально парсер умеет обрабатывать только строки, целочисленные и булевые аргументы. Вам не запрещается добавить произвольные типы и расширить функционал.

### Тесты

Как и в предыдущих лабораторный работах, в качестве тестового фреймворка используется [GoogleTest](https://google.github.io/googletest/). Каждый тест представляет из себя функцию, в рамках которой проверяются некоторые условия за счет [assert'ов](https://google.github.io/googletest/reference/assertions.html).  Предложенные Вам тесты используют только два различных assert'a:

    - ASSERT_TRUE  - проверяет, что выражение является истинным (в противном случае тест неуспешен)
    - ASSERT_EQ    - проверяет, что аргументы эквивалентны

Например, тест

```cpp
TEST(ArgParserTestSuite, StringTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}
```

Проверяет, что

 - Вызов `parser.Parse(SplitString("app --param1=value1"))` вернет `true`
 - Вызов `parser.GetStringValue("param1")` вернет `"value1"`

## Реализация

Реализация должна находится в библиотеке [argparser](lib/argparser/CMakeLists.txt). Изначально в библиотеке есть один класс, при необходимости добавляйте новые файлы, классы, функции и т.д. в библиотеку.

### Примеры запуска

Пример программы с использованием парсера находится в [bin](bin/main.cpp). Программа умеет складывать или умножать переданные ей аргументы.

*labwork4 --sum 1 2 3 4 5*

*labwork4 --mult 1 2 3 4 5*

## NB

1. Выполнение работы подразумевает только базовые знания о классах. Не запрещается использовать шаблоны, виртуальные функции и т.д. Однако для этого надо хорошо понимать как они работают и быть готовыми к вопросам.

2. Данная лабораторная работа может быть переиспользована в следующих, в качестве библиотеки для работы с аргументами командной строки.

## Deadline

1. 21.11.23 24:00 - 0.8
2. 28.11.23 24:00 - 0.65
3. 05.12.23 24:00 - 0.5
