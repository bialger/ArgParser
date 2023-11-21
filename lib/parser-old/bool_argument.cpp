#include "bool_argument.hpp"

#include <cstring>

BoolArgument::BoolArgument() {
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

BoolArgument::BoolArgument(ArgumentInformation info) {
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

void BoolArgument::ValidateArgument(char* candidate) {
  if (strcmp(candidate, info_.short_key) == 0 || strcmp(candidate, info_.long_key) == 0) {
    value_ = true;
    value_status_ = ArgumentParsingStatus::kSuccess;
  }
}

bool BoolArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus BoolArgument::GetValueStatus() const {
  return value_status_;
}


ArgumentType BoolArgument::GetType() const {
  return info_.type;
}

const char* BoolArgument::GetName() const {
  return info_.name;
}

bool BoolArgument::GetIsRequired() const {
  return info_.is_required;
}
