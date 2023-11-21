#include "string_argument.hpp"

#include <cstring>

StringArgument::StringArgument() {
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

StringArgument::StringArgument(ArgumentInformation info) {
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

StringArgument::StringArgument(const StringArgument& other) {
  value_ = other.value_;
  value_status_ = other.value_status_;
  info_ = other.info_;
}

StringArgument& StringArgument::operator=(const StringArgument& other) {
  if (this == &other) {
    return *this;
  }

  value_ = other.value_;
  value_status_ = other.value_status_;
  info_ = other.info_;

  return *this;
}

StringArgument::~StringArgument() {
  delete[] value_;
}

void StringArgument::ValidateArgument (char* candidate, char* value, bool is_last) {
  bool is_short = strcmp(candidate, info_.short_key) == 0 && !is_last;
  bool is_long = strncmp(candidate, info_.long_key, strlen(info_.long_key)) == 0;

  if (!(is_short || is_long) ||
      value_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_value = is_short ? value : candidate + strlen(info_.long_key);

  if (pre_value[0] == '\'' || pre_value[0] == '"') {
    ++pre_value;
    *(strrchr(pre_value, '\0') - 1) = '\0';
  }

  value_ = new char[strlen(pre_value) + 1];
  strcpy(value_, pre_value);
  value_status_ = ArgumentParsingStatus::kSuccess;

  if (!info_.IsGood(value)) {
    value_ = kError;
    value_status_ = ArgumentParsingStatus::kBrokenArgument;
  }
}

char* StringArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus StringArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentType StringArgument::GetType() const {
  return info_.type;
}

const char* StringArgument::GetName() const {
  return info_.name;
}

bool StringArgument::GetIsRequired() const {
  return info_.is_required;
}
