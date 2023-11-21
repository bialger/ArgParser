#include "ConcreteArgument.hpp"
#include <limits>

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

  if (info_.minimum_values == 0 && !info_.has_default) {
    info_.minimum_values = 1;
  }

  if (!info_.has_store_values) {
    stored_values_ = new std::vector<T>;
  }
}

template<typename T>
ArgumentParser::ConcreteArgument<T>::~ConcreteArgument() {
  if (!info_.has_store_values) {
    delete stored_values_;
  }
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
std::vector<size_t> ArgumentParser::ConcreteArgument<T>::ValidateArgument(
    const std::vector<std::string>& argv, size_t position) {
  std::vector<size_t> used_values;
  if (stored_values_->empty()) {
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  while (used_values.empty() || info_.is_multi_value) {
    position = ObtainValue(argv, used_values, position);

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

  return used_values;
}

template<>
size_t ArgumentParser::ConcreteArgument<bool>::ObtainValue(const std::vector<std::string>& argv,
                                                           std::vector<size_t>& used_values,
                                                           size_t position) {
  std::string value_string = argv[position];
  used_values.push_back(position);

  if (used_values.size() == 1 && !info_.is_positional) {
    value_string = "true";
    size_t equals_index = argv[position].find('=');

    if (equals_index != std::string::npos) {
      value_string = argv[position].substr(equals_index + 1);
    }
  }

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
                                                              std::vector<size_t>& used_values,
                                                              size_t position) {
  std::string value_string = argv[position];
  used_values.push_back(position);

  if (used_values.size() == 1 && !info_.is_positional) {
    size_t equals_index = argv[position].find('=');

    if (equals_index != std::string::npos) {
      value_string = argv[position].substr(equals_index + 1);
    } else {
      ++position;
      used_values.clear();
      used_values.push_back(position);
      value_string = argv[position];
    }
  }

  int32_t save_errno = errno;
  errno = 0;
  char* end;
  int64_t pre_value = std::strtoll(value_string.c_str(), &end, 0);

  if (end == value_string.c_str() || *end != '\0' ||
      ((pre_value <= std::numeric_limits<int32_t>::min() || pre_value >= std::numeric_limits<int32_t>::max()) &&
          errno == ERANGE)) {
    errno = save_errno;
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  } else {
    value_ = static_cast<int32_t>(pre_value);
    value_status_ = ArgumentParsingStatus::kSuccess;
  }

  return position;
}

template<>
size_t ArgumentParser::ConcreteArgument<std::string>::ObtainValue(const std::vector<std::string>& argv,
                                                                  std::vector<size_t>& used_values,
                                                                  size_t position) {
  std::string value_string = argv[position];
  used_values.push_back(position);

  if (used_values.size() == 1 && !info_.is_positional) {
    size_t equals_index = argv[position].find('=');

    if (equals_index != std::string::npos) {
      value_string = argv[position].substr(equals_index + 1);
    } else {
      ++position;
      used_values.clear();
      used_values.push_back(position);
      value_string = argv[position];
    }
  }

  if (info_.IsGood(value_string.data())) {
    value_ = value_string;
  } else {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  }

  return position;
}

template<>
size_t ArgumentParser::ConcreteArgument<CompositeString>::ObtainValue(const std::vector<std::string>& argv,
                                                                      std::vector<size_t>& used_values,
                                                                      size_t position) {
  // TODO: implement composite argument handling
  return position;
}

