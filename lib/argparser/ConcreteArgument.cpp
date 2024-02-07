#include <limits>

#include "ConcreteArgument.hpp"

template
class ArgumentParser::ConcreteArgument<bool>;
template
class ArgumentParser::ConcreteArgument<int32_t>;
template
class ArgumentParser::ConcreteArgument<std::string>;
template
class ArgumentParser::ConcreteArgument<CompositeString>;

template<typename T>
ArgumentParser::ConcreteArgument<T>::ConcreteArgument(ArgumentInformation info,
                                                      T default_value,
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

template<typename T>
T ArgumentParser::ConcreteArgument<T>::GetValue(size_t index) const {
  if (!info_.has_store_values) {
    return value_;
  }

  return stored_values_->at(index);
}

template<typename T>
ArgumentParser::ArgumentParsingStatus ArgumentParser::ConcreteArgument<T>::GetValueStatus() const {
  return value_status_;
}

template<typename T>
const std::string& ArgumentParser::ConcreteArgument<T>::GetType() const {
  return info_.type;
}

template<typename T>
const ArgumentParser::ArgumentInformation& ArgumentParser::ConcreteArgument<T>::GetInfo() const {
  return info_;
}

template<typename T>
size_t ArgumentParser::ConcreteArgument<T>::GetUsedValues() const {
  return value_counter_;
}

template<typename T>
bool ArgumentParser::ConcreteArgument<T>::CheckLimit() {
  if (value_counter_ < info_.minimum_values) {
    value_status_ = ArgumentParsingStatus::kInsufficientArguments;
    return false;
  }

  if (value_status_ == ArgumentParsingStatus::kInsufficientArguments) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  return true;
}

template<typename T>
void ArgumentParser::ConcreteArgument<T>::ClearStored() {
  stored_values_->clear();
  value_counter_ = 0;

  if (stored_value_ != nullptr) {
    *stored_value_ = default_value_;
  }
}

template<typename T>
std::vector<size_t> ArgumentParser::ConcreteArgument<T>::ValidateArgument(const std::vector<std::string>& argv,
                                                                          size_t position) {
  std::vector<size_t> used_positions;
  if (stored_values_->empty()) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  while (used_positions.empty() || info_.is_multi_value) {
    std::string value_string = argv[position];
    used_positions.push_back(position);

    if (argv[position][0] == '-') {
      size_t equals_index = argv[position].find('=');

      if (equals_index != std::string::npos) {
        value_string = argv[position].substr(equals_index + 1);
      } else if (std::is_same<T, bool>::value) {
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

template<>
size_t ArgumentParser::ConcreteArgument<bool>::ObtainValue(const std::vector<std::string>& argv,
                                                           std::string& value_string,
                                                           std::vector<size_t>& used_values,
                                                           size_t position) {
  if (value_string == "0" || value_string == "false") {
    value_ = false;
  } else if (value_string == "1" || value_string == "true") {
    value_ = true;
  } else {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  }

  return position;
}

template<>
size_t ArgumentParser::ConcreteArgument<int32_t>::ObtainValue(const std::vector<std::string>& argv,
                                                              std::string& value_string,
                                                              std::vector<size_t>& used_values,
                                                              size_t position) {
  int32_t save_errno = errno;
  errno = 0;
  char* end;
  int64_t pre_value = std::strtoll(value_string.c_str(), &end, 0);

  if (end == value_string.c_str() || *end != '\0' ||
      ((pre_value <= std::numeric_limits<int32_t>::min() || pre_value >= std::numeric_limits<int32_t>::max()) &&
          errno == ERANGE) || !info_.IsGood(value_string)) {
    errno = save_errno;
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  } else {
    value_ = static_cast<int32_t>(pre_value);
  }

  return position;
}

template<>
size_t ArgumentParser::ConcreteArgument<std::string>::ObtainValue(const std::vector<std::string>& argv,
                                                                  std::string& value_string,
                                                                  std::vector<size_t>& used_values,
                                                                  size_t position) {
  if (info_.IsGood(value_string)) {
    value_ = value_string;
  } else {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  }

  return position;
}

template<>
size_t ArgumentParser::ConcreteArgument<CompositeString>::ObtainValue(const std::vector<std::string>& argv,
                                                                      std::string& value_string,
                                                                      std::vector<size_t>& used_values,
                                                                      size_t position) {
  if (!info_.Validate(value_string)) {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
    return position;
  }

  size_t current = position + (info_.IsGood(value_string) ? 0 : 1);

  while (current < argv.size() && argv[current][0] != '-' &&
      value_status_ != ArgumentParsingStatus::kInvalidArgument &&
      value_string.size() < 256 &&
      !info_.IsGood(value_string)) {
    value_string += " ";
    value_string += argv[current];

    if (!info_.Validate(value_string)) {
      value_status_ = ArgumentParsingStatus::kInvalidArgument;
      break;
    }

    ++current;
  }

  if (info_.Validate(value_string) && info_.IsGood(value_string)) {
    value_ = value_string.c_str();
    position = current;
    size_t start_position = used_values.back();

    for (size_t i = start_position; i <= current; ++i) {
      used_values.push_back(i);
    }
  } else {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  }

  return position;
}
