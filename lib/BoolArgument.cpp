#include "BoolArgument.hpp"

#include <cstring>

ArgumentParser::BoolArgument::BoolArgument() {
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

ArgumentParser::BoolArgument::BoolArgument(ArgumentInformation info) {
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

void ArgumentParser::BoolArgument::ValidateArgument(char* candidate) {
  if (strcmp(candidate, info_.short_key) == 0 || strcmp(candidate, info_.long_key) == 0) {
    value_ = true;
    value_status_ = ArgumentParsingStatus::kSuccess;
  }
}

bool ArgumentParser::BoolArgument::GetValue() const {
  return value_;
}

ArgumentParser::ArgumentParsingStatus ArgumentParser::BoolArgument::GetValueStatus() const {
  return value_status_;
}


ArgumentParser::ArgumentType ArgumentParser::BoolArgument::GetType() const {
  return info_.type;
}

const char* ArgumentParser::BoolArgument::GetName() const {
  return info_.name;
}

bool ArgumentParser::BoolArgument::GetIsRequired() const {
  return info_.is_required;
}