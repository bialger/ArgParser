# Требования к продукту

В этом документе описаны требования к продукту — парсеру аргументов командной строки.

## Функциональные требования

Поскольку необходимо разработать библиотеку, подразумевающую дальнейшее использование
в качестве компонента других программ, требования не подразумевают прямо некий формат
UI, а описывают взаимодействие программистов, использующих библиотеку с ней.

### Требования к формату входных данных

* Входные данные должны представлять собой строки, несущие всю необходимую информацию
  об аргументах, которые надо распарсить, а также непосредственно строку вызова
  программы.
* Входные данные могут быть представлены в разных форматах: как вектор строк, так и динамический
  массив C-style строк.
* Формат входных данных должен соответствовать [тестам](../../../../tests/argparser_unit_tests.cpp)
  из соответствующего файла. 
  То есть должны быть реализованы все функции из тестов
  со всеми возможными комбинациями аргументов функций из представленных.

### Требования к формату выходных данных

* Выходные данные — исключительно возвращаемые значения функций библиотеки.
* Формат выходных данных должен соответствовать [тестам](../../../../tests/argparser_unit_tests.cpp)
  из соответствующего файла. 
  То есть должны быть реализованы все функции из тестов
  с необходимыми типами возвращаемых значений.

### Требования к функциональности продукта

* Итоговый продукт должен проходить все [тесты](../../../../tests/argparser_unit_tests.cpp) —
  главный приоритет. Первые 15 — строго обязательно.
* Кроме того, должна корректно работать программа, описанная в
  [main.cpp](../../../../bin/main.cpp).
* Продукт должен настолько, насколько это возможно, соответствовать стандарту POSIX.
* К предоставленным тестам должны быть добавлены дополнительные, проверяющие
  соответствие стандарту POSIX.
* Полученные значения аргументов должны валидироваться (на корректность).
* Должна быть реализована поддержка "сложного аргумента" — например, имени файла
  с пробелами (последнее опционально).

## Прочие требования

### Требования к быстродействию

* Требования к быстродействию отсутствуют исходя из специфики проблемы. Парсер
  аргументов командной строки — модуль, который запускается всего один раз за всё
  время работы программы, и всегда получает небольшое количество данных. Значит,
  его быстродействие слабо влияет на быстродействие системы.

### Требования к безопасности

* Требования безопасности ограничены валидацией значений аргументов на корректность,
  поскольку выявление угроз безопасности не относится к зоне ответственности парсера
  аргументов командной строки.

### Требования к масштабируемости

* Масштабируемость не требуется исходя из специфики парсера аргументов командной
  строки.

### Требования к возможности доработки

* Продукт должен обладать гибкой архитектурой с возможностью доработки и внесения
  нового функционала.
