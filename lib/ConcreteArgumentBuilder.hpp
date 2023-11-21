#ifndef CONCRETEARGUMENTBUILDER_HPP_
#define CONCRETEARGUMENTBUILDER_HPP_

#include <sstream>

#include "ArgumentBuilder.h"
#include "ConcreteArgument.hpp"

namespace ArgumentParser {

template<typename T>
class ConcreteArgumentBuilder : public ArgumentBuilder {
 public:
  ConcreteArgumentBuilder() = delete;

  ConcreteArgumentBuilder(char short_name, const char* long_name, const char* description) {
    info_ = ArgumentInformation();
    info_.short_key = short_name;
    info_.long_key = long_name;
    info_.description = description;
    info_.type = typeid(T).name();
    default_value_ = T();
    stored_value_ = nullptr;
    stored_values_ = nullptr;
  }

  ConcreteArgumentBuilder(const ConcreteArgumentBuilder& other) = delete;
  ConcreteArgumentBuilder& operator=(const ConcreteArgumentBuilder& other) = delete;
  ~ConcreteArgumentBuilder() override = default;

  ConcreteArgumentBuilder& MultiValue(int min = 0) {
    info_.is_multi_value = true;
    info_.minimum_values = min;
    return *this;
  }

  ConcreteArgumentBuilder& Positional() {
    info_.is_positional = true;
    return *this;
  }

  ConcreteArgumentBuilder& StoreValue(T& value) {
    info_.has_store_value = true;
    stored_value_ = &value;
    return *this;
  }

  ConcreteArgumentBuilder& StoreValues(std::vector<T>& values) {
    info_.has_store_values = true;
    stored_values_ = &values;
    return *this;
  }

  ConcreteArgumentBuilder& Default(T value) {
    info_.has_default = true;
    default_value_ = value;
    return *this;
  }

  ConcreteArgumentBuilder& AddValidate(bool (* validate)(std::string&)) {
    info_.Validate = validate;
    return *this;
  }

  ConcreteArgumentBuilder& AddIsGood(bool (* is_good)(std::string&)) {
    info_.IsGood = is_good;
    return *this;
  }

  Argument* build() override {
    if (std::is_same<T, bool>::value && !info_.has_default) {
      info_.has_default = true;
    }

    if (info_.minimum_values == 0 && !info_.has_default) {
      info_.minimum_values = 1;
    }

    return new ConcreteArgument<T>(info_, default_value_, stored_value_, stored_values_);
  }

  const ArgumentInformation& GetInfo() const override {
    return info_;
  }

  std::string GetDefaultValue() const override {
    std::ostringstream stream;
    stream << default_value_;
    return stream.str();
  }
 private:
  ArgumentInformation info_;
  T default_value_;
  T* stored_value_;
  std::vector<T>* stored_values_;
};

} // namespace ArgumentParser

#endif //CONCRETEARGUMENTBUILDER_HPP_
