#include "long_argument.hpp"
#include "lib/basic/basic_functions.hpp"

#include <limits>
#include <cstring>

LongArgument::LongArgument() {
  value_ = 0;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

LongArgument::LongArgument(ArgumentInformation info) {
  value_ = std::numeric_limits<uint64_t>::max();
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

void LongArgument::ValidateArgument(char* candidate, char* value, bool is_last) {
  bool is_short = strcmp(candidate, info_.short_key) == 0 && !is_last;
  bool is_long = strncmp(candidate, info_.long_key, strlen(info_.long_key)) == 0;

  if (is_short || is_long) {
    char* string_value = is_short ? value : candidate + strlen(info_.long_key);

    if (string_value[0] == '\'' || string_value[0] == '"') {
      ++string_value;
      *(strrchr(string_value, '\0') - 1) = '\0';
    }

    int64_t pre_value = IntFromString(string_value, -1, 0);

    if (pre_value != -1l) {
      value_ = static_cast<uint64_t>(pre_value);
      value_status_ = ArgumentParsingStatus::kSuccess;
    } else {
      value_status_ = ArgumentParsingStatus::kBrokenArgument;
    }
  }
}

uint64_t LongArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus LongArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentType LongArgument::GetType() const {
  return info_.type;
}

const char* LongArgument::GetName() const {
  return info_.name;
}

bool LongArgument::GetIsRequired() const {
  return info_.is_required;
}
