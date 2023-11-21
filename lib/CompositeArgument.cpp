#include <cstring>
#include "CompositeArgument.hpp"
#include "BasicFunctions.hpp"

ArgumentParser::CompositeArgument::CompositeArgument() {
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

ArgumentParser::CompositeArgument::CompositeArgument(ArgumentInformation info) {
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

ArgumentParser::CompositeArgument::CompositeArgument(const CompositeArgument& other) {
  value_ = other.value_;
  value_status_ = other.value_status_;
  info_ = other.info_;
}

ArgumentParser::CompositeArgument& ArgumentParser::CompositeArgument::operator=(const CompositeArgument& other) {
  if (this == &other) {
    return *this;
  }

  value_ = other.value_;
  value_status_ = other.value_status_;
  info_ = other.info_;

  return *this;
}

ArgumentParser::CompositeArgument::~CompositeArgument() {
  delete[] value_;
}

void ArgumentParser::CompositeArgument::ValidateArgument(char** argv,
                                         int32_t argc,
                                         char* candidate,
                                         char* value,
                                         int32_t position) {
  bool is_short = strcmp(candidate, info_.short_key) == 0 && position != argc - 1;
  bool is_long = strncmp(candidate, info_.long_key, strlen(info_.long_key)) == 0;

  if (!(is_short || is_long) ||
      value_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_value = is_short ? value : candidate + strlen(info_.long_key);

  if (strlen(pre_value) > 1) {
    if (strncmp(pre_value, "./", 2) == 0 || strncmp(pre_value, "C:", 2) == 0 ||
        strncmp(pre_value, ".\\", 2) == 0 || (!IsWindows() && strncmp(pre_value, "../", 3) == 0)
        || pre_value[0] == '/') {
      value_ = new char[strlen(pre_value) + 1];
      strcpy(value_, pre_value);
    } else if (strncmp(pre_value, "file://", 7) == 0) {
      value_ = new char[strlen(pre_value + 7) + 1];
      strcpy(value_, pre_value + 7);
    }

    if (value_ != kError) {
      bool is_real_file = false;
      int32_t current = position;

      if (info_.Validate(value_)) {
        is_real_file = info_.IsGood(value_);
      } else {
        value_status_ = ArgumentParsingStatus::kInvalidArgument;
      }

      while (!is_real_file && current < argc - 1 &&
          value_status_ != ArgumentParsingStatus::kInvalidArgument &&
          strlen(value_) < 256) {
        size_t len = strlen(value_) + strlen(argv[current + 1]) + 2;
        char* new_candidate = new char[len];
        std::memset(new_candidate, 0, len);
        strcpy(new_candidate, value_);
        strcat(new_candidate, " ");
        strcat(new_candidate, argv[current + 1]);
        delete[] value_;
        value_ = new_candidate;

        if (info_.Validate(value_)) {
          is_real_file = info_.IsGood(value_);
        } else {
          value_status_ = ArgumentParsingStatus::kInvalidArgument;
        }

        ++current;
      }

      if (is_real_file) {
        value_status_ = ArgumentParsingStatus::kSuccess;
      } else {
        delete[] value_;
        value_ = kError;
        value_status_ = ArgumentParsingStatus::kInvalidArgument;
      }
    }
  }

  if (value_status_ != ArgumentParsingStatus::kSuccess) {
    char* simple_value = new char[strlen(pre_value) + (IsWindows() ? 1 : 3)];
    std::memset(simple_value, 0, strlen(pre_value) + (IsWindows() ? 1 : 3));
    strcat(simple_value, IsWindows() ? "" : "./");
    strcat(simple_value, pre_value);

    if (info_.Validate(simple_value)) {
      if (info_.IsGood(simple_value)) {
        value_ = simple_value;
        value_status_ = ArgumentParsingStatus::kSuccess;
      }
    }
  }

  if (value_status_ == ArgumentParsingStatus::kNoArgument) {
    value_status_ = ArgumentParsingStatus::kInvalidArgument;
  }
}

char* ArgumentParser::CompositeArgument::GetValue() const {
  return value_;
}

ArgumentParser::ArgumentParsingStatus ArgumentParser::CompositeArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentParser::ArgumentType ArgumentParser::CompositeArgument::GetType() const {
  return info_.type;
}

const char* ArgumentParser::CompositeArgument::GetName() const {
  return info_.name;
}

bool ArgumentParser::CompositeArgument::GetIsRequired() const {
  return info_.is_required;
}
