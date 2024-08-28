#ifndef CONCRETEARGUMENT_HPP_
#define CONCRETEARGUMENT_HPP_

#include "Argument.hpp"
#include "ArgParserConcepts.hpp"

namespace ArgumentParser {

template<ProperArgumentType T>
struct NonMemberParsingResult {
  bool success = true;
  T value = T();
};

template<ProperArgumentType T>
class ConcreteArgument final : public Argument {
 public:
  ConcreteArgument() = delete;
  ConcreteArgument(const ArgumentInformation& info,
                   const T& default_value,
                   T* stored_value,
                   std::vector<T>* stored_values);
  [[nodiscard]] T GetValue(size_t index) const;
  std::vector<size_t> ValidateArgument(const std::vector<std::string>& argv, size_t position) override;
  bool CheckLimit() override;
  [[nodiscard]] ArgumentParsingStatus GetValueStatus() const override;
  [[nodiscard]] const std::string_view& GetType() const override;
  [[nodiscard]] const ArgumentInformation& GetInfo() const override;
  [[nodiscard]] size_t GetUsedValues() const override;
  void ClearStored() override;
 protected:
  size_t ObtainValue(const std::vector<std::string>& argv, std::string& value_string,
                     std::vector<size_t>& used_values, size_t position) override;

 private:
  ArgumentInformation info_;
  ArgumentParsingStatus value_status_;
  size_t value_counter_;
  T value_;
  T default_value_;
  T* stored_value_;
  std::vector<T>* stored_values_;
};

template<ProperArgumentType T>
ConcreteArgument<T>::ConcreteArgument(const ArgumentInformation& info,
                                      const T& default_value,
                                      T* stored_value,
                                      std::vector<T>* stored_values) {
  info_ = info;
  value_ = default_value;
  value_counter_ = 0;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  default_value_ = default_value;
  stored_value_ = stored_value;
  stored_values_ = stored_values;
}

template<ProperArgumentType T>
T ConcreteArgument<T>::GetValue(size_t index) const {
  if (!info_.has_store_values) {
    return value_;
  }

  return stored_values_->at(index);
}

template<ProperArgumentType T>
ArgumentParsingStatus ConcreteArgument<T>::GetValueStatus() const {
  return value_status_;
}

template<ProperArgumentType T>
const std::string_view& ConcreteArgument<T>::GetType() const {
  return info_.type;
}

template<ProperArgumentType T>
const ArgumentInformation& ConcreteArgument<T>::GetInfo() const {
  return info_;
}

template<ProperArgumentType T>
size_t ConcreteArgument<T>::GetUsedValues() const {
  return value_counter_;
}

template<ProperArgumentType T>
bool ConcreteArgument<T>::CheckLimit() {
  if (value_counter_ < info_.minimum_values) {
    value_status_ = ArgumentParsingStatus::kInsufficientArguments;
    return false;
  }

  if (value_status_ == ArgumentParsingStatus::kInsufficientArguments) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  return true;
}

template<ProperArgumentType T>
void ConcreteArgument<T>::ClearStored() {
  stored_values_->clear();
  value_counter_ = 0;

  if (stored_value_ != nullptr) {
    *stored_value_ = default_value_;
  }
}

template<ProperArgumentType T>
std::vector<size_t> ConcreteArgument<T>::ValidateArgument(const std::vector<std::string>& argv,
                                                          size_t position) {
  std::vector<size_t> used_positions;
  if (stored_values_->empty()) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  while (used_positions.empty() || info_.is_multi_value) {
    std::string value_string = argv[position];
    used_positions.push_back(position);

    if (argv[position][0] == '-') {
      const size_t equals_index = argv[position].find('=');

      if (equals_index != std::string::npos) {
        value_string = argv[position].substr(equals_index + 1);
      } else if (std::is_same_v<T, bool>) {
        value_string = "true";
      } else {
        ++position;
        used_positions.push_back(position);
        value_string = argv[position];
      }
    }

    position = ObtainValue(argv, value_string, used_positions, position);

    ++position;
    ++value_counter_;

    stored_values_->push_back(value_);

    if (position >= argv.size() || argv[position][0] == '-') {
      break;
    }
  }

  CheckLimit();

  if (info_.has_store_value) {
    *stored_value_ = value_;
  }

  return used_positions;
}

}

#define PassArgumentTypes(...) ArgumentParser::ArgumentTypes<__VA_ARGS__>{}

/**\n This macro adds a definition of the parsing method for ConcreteArgument<Type>. \n
 * Note that this macro creates such definition for types with simple logics. */

#define AddArgumentType(Type, ParsingFunction) \
template<> \
size_t ArgumentParser::ConcreteArgument<Type>::ObtainValue(const std::vector<std::string>& argv, \
std::string& value_string, \
std::vector<size_t>& used_values, \
size_t position) { \
  NonMemberParsingResult<Type> result = ParsingFunction(value_string); \
  \
  if (result.success) {\
    value_ = result.value; \
  } else { \
    value_status_ = ArgumentParsingStatus::kInvalidArgument;\
  }\
  \
  return position; \
}

#endif //CONCRETEARGUMENT_HPP_
