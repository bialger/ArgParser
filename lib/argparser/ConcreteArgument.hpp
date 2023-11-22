#ifndef CONCRETEARGUMENT_HPP_
#define CONCRETEARGUMENT_HPP_

#include "Argument.h"

namespace ArgumentParser {

template<typename T>
class ConcreteArgument : public Argument {
 public:
  ConcreteArgument() = delete;
  ConcreteArgument(ArgumentInformation info, T default_value, T* stored_value, std::vector<T>* stored_values);
  ~ConcreteArgument() = default;
  T GetValue(size_t index) const;
  std::vector<size_t> ValidateArgument(const std::vector<std::string>& argv, size_t position) override;
  bool CheckLimit() override;
  ArgumentParsingStatus GetValueStatus() const override;
  const std::string& GetType() const override;
  const ArgumentInformation& GetInfo() const override;
  size_t GetUsedValues() const override;
 private:
  ArgumentInformation info_;
  ArgumentParsingStatus value_status_;
  size_t value_counter_;
  T value_;
  T default_value_;
  T* stored_value_;
  std::vector<T>* stored_values_;

  size_t ObtainValue(const std::vector<std::string>& argv, std::string& value_string,
                     std::vector<size_t>& used_values, size_t position) override;
};

}

#endif //CONCRETEARGUMENT_HPP_
