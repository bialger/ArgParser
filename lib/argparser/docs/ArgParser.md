# Документация класса ArgParser

Этот класс является основным классом модуля, именно с ним обычно взаимодействует
пользователь. Он предоставляет следующие возможности: добавление обрабатываемого
аргумента (любого из указанных) с указанным ключом, парсинг набора аргументов командной
строки, а также добавление аргумента указанного типа. Кроме того, он имеет функцию
составления справки и обработки ошибки в синтаксисе аргументов командной строки, а
также их вывода. Находится в пространстве имён `ArgumentParser`.

## Объявление и определение

Объявлен в [ArgParser.hpp](../ArgParser.hpp). <br>
Определен там же (шаблонные функции) и в [ArgParser.cpp](../ArgParser.cpp)

## Зависимости

Зависит от [BasicFunctions.hpp](../basic/BasicFunctions.hpp),
[ConcreteArgument.hpp](../ConcreteArgument.hpp),
[ConcreteArgumentBuilder.hpp](../ConcreteArgumentBuilder.hpp),
[ArgParserConcepts.hpp](../ArgParserConcepts.hpp).

## Связанные документы

* Документация класса [ConcreteArgumentBuilder](ConcreteArgumentBuilder.md) —
  информация о задании параметров аргументов, их типах и логике парсинга.

## Публичные поля

Класс не имеет публичных полей.

## Публичные методы

### ArgParser

Принимает имя программы и типы добавленных пользователем аргументов.
Типы добавленных аргументов должны быть представлены в виде `ArgumentTypes<Args ...>`, где Args... — типы аргументов.
Аргументы должны удовлетворять концепту `ProperArgumentType`, то есть:
* Иметь конструктор без параметров и конструктор копирования
* Иметь оператор присваивания с копированием
* Должен быть определен оператор `std::ostream& operator<<(std::ostream& os, const T& t)`

Ожидается вызов пользователем именно его.

```cpp
template<ProperArgumentType ... Args>
ArgParser(const std::string& name = "", ArgumentTypes<Args ...> types = {});
```

Удален конструктор копирования (как и оператор присваивания)

```cpp
ArgParser(const ArgParser& other) = delete;
ArgParser& operator=(const ArgParser& other) = delete;
```

### Parse

Функция парсинга. Вызывает приватный метод Parse_, который выполняет парсинг
аргументов командной строки, переданных в виде `std::vector` из `std::string`
(параметр *args*). Принимает `ConditionalOutput` параметр error_output, по умолчанию
не выводящий ошибки. Возвращает `true` при успешном завершении парсинга,
и `false` в случае его провала.

Синтаксис аргументов частично POSIX-совместимый (поддерживается передача значений
через "=" или пробел после ключа, коротки и длинные ключи, комбинация коротких
ключей, остановка парсинга по аргументу "--", получение множества значений в аргумент).

```cpp
bool Parse(const std::vector<std::string>& args, ConditionalOutput error_output = {std::cout, false});
```

Перегрузка Parse с тем же функционалом, но принимающая *C-style array* из
*C-style string* длиной *argc* вместо `std::vector` из `std::string`.

```cpp
bool Parse(int argc, char** argv, ConditionalOutput error_output = {std::cout, false});
```

### Help

Функция, проверяющая необходимость вывода помощи. Возвращает `true` при получении
положительного значения HelpArgument, и `false` в случае его отсутствия или
негативного значения после парсинга.

```cpp
[[nodiscard]] bool Help() const;
```

### HelpDescription

Функция, возвращающая `std::string`, содержащую помощь для пользователя.

```cpp
[[nodiscard]] std::string HelpDescription() const;
```

Пример вывода возвращаемого значения:

```text
My Parser
Some Description about program

OPTIONS:
-i,  --input=<string>:  File path for input file [repeated, min args = 1]
     --number=<int>:  Some Number
-s,  --flag1:  Use some logic [default = true]
-p,  --flag2:  Use some logic

-h,  --help:  Display this help and exit
```

### GetValue<T\>

Шаблонная функция, возвращающая значение типа `T` аргумента с длинным ключом
*long_name*, полученное на *index* позиции (с нуля, по умолчанию - `0`). В случае
вызова функции с типом, не принадлежащему к поддерживаемым, при не добавленном
значении *long_name* или индекса, большего, чем количество полученных значений,
выбрасывается исключение `std::out_of_range`. В случае вызова до применения функции
парсинга, выбрасывается исключение `std::runtime_error`.

```cpp
template<ProperArgumentType T>
[[nodiscard]] T GetValue(const std::string_view& long_name, size_t index = 0) const;
```

### AddArgument<T\>

Шаблонная функция, возвращающая ссылку на экземпляр класса
[ConcreteArgumentBuilder<T\>](ConcreteArgumentBuilder.md), который будет строить
аргумент с переданными параметрами. 
Принимает `char` значение короткого ключа, `std::string_view` значение длинного ключа 
и, опционально, `std::string` описание ключа (по умолчанию — пустая строка).

```cpp
template<ProperArgumentType T>
ConcreteArgumentBuilder<T>& AddArgument(char short_name, const std::string_view& long_name, const std::string& description = "");
```

Эта перегрузка не принимает никакого значения короткого ключа.

```cpp
template<ProperArgumentType T>
ConcreteArgumentBuilder<T>& AddArgument(const std::string_view& long_name, const std::string& description = "");
```

### AddFlag, AddShortArgument, ... AddStringArgument, AddCompositeArgument

Функции-псевдонимы для соответственно `AddArgument<bool>`, `AddArgument<int16_t>`, ...
`AddArgument<std::string>` и `AddArgument<CompositeString>`

### AddHelp

Функция, вызывающая соответственную перегрузку `AddArgument<bool>`, возвращающая
результат и запоминающая аргумент помощи.

```cpp
ConcreteArgumentBuilder<bool>& AddHelp(char short_name, const std::string_view& long_name, const std::string& description = "");
```

Эта перегрузка не принимает никакого значения короткого ключа.

```cpp
ConcreteArgumentBuilder<bool>& AddHelp(const std::string_view& long_name, const std::string& description);
```

### SetAliasForType<T\>

Функция, переопределяющая псевдоним для типа `T`. 
Разрешены только латинские буквы и цифры.

```cpp
template<ProperArgumentType T>
void SetAliasForType(const std::string& alias);
```

### GetFlag, GetShortValue, ... GetStringValue, GetCompositeValue

Функции-псевдонимы для соответственно `GetValue<bool>`, `GetValue<int16_t>`,
`GetValue<std::string>` и `GetValue<CompositeString>`
