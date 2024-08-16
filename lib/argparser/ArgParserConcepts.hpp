#ifndef ARGPARSERCONCEPTS_HPP_
#define ARGPARSERCONCEPTS_HPP_

#include <iostream>
#include <concepts>

template<typename T>
concept Writeable = requires(T t, std::ostream& os) {
  { os << t } -> std::same_as<std::ostream&>;
};

template<typename T>
concept ProperArgumentType = std::is_default_constructible_v<T> && std::is_copy_constructible_v<T>
    && std::is_copy_assignable_v<T> && Writeable<T>;

#endif //ARGPARSERCONCEPTS_HPP_
