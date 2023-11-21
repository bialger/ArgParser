#include "LongArgument.hpp"
#include "BasicFunctions.hpp"

#include <limits>
#include <cstring>
#include <cstdlib>
#include <cerrno>

ArgumentParser::LongArgument::LongArgument() {
  value_ = 0;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = ArgumentInformation{};
}

ArgumentParser::LongArgument::LongArgument(ArgumentInformation info) {
  value_ = std::numeric_limits<uint64_t>::max();
  value_status_ = ArgumentParsingStatus::kNoArgument;
  info_ = info;
}

void ArgumentParser::LongArgument::ValidateArgument(char* candidate, char* value, bool is_last) {
  bool is_short = strcmp(candidate, info_.short_key) == 0 && !is_last;
  bool is_long = strncmp(candidate, info_.long_key, strlen(info_.long_key)) == 0;

  if (is_short || is_long) {
    char* string_value = is_short ? value : candidate + strlen(info_.long_key);

    if (string_value[0] == '\'' || string_value[0] == '"') {
      ++string_value;
      *(strrchr(string_value, '\0') - 1) = '\0';
    }

    int64_t pre_value1 = strtoll(string_value, nullptr, 0);

    int32_t save_errno = errno;
    errno = 0;
    char* end;
    int64_t pre_value = std::strtoll(string_value, &end, 0);

    if (end == string_value || *end != '\0' ||
        ((pre_value == std::numeric_limits<int64_t>::min() || pre_value == std::numeric_limits<int64_t>::max()) &&
        errno == ERANGE)) {
      pre_value = -1;
      errno = save_errno;
    }

    if (pre_value != -1l) {
      value_ = static_cast<uint64_t>(pre_value);
      value_status_ = ArgumentParsingStatus::kSuccess;
    } else {
      value_status_ = ArgumentParsingStatus::kBrokenArgument;
    }
  }
}

uint64_t ArgumentParser::LongArgument::GetValue() const {
  return value_;
}

ArgumentParser::ArgumentParsingStatus ArgumentParser::LongArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentParser::ArgumentType ArgumentParser::LongArgument::GetType() const {
  return info_.type;
}

const char* ArgumentParser::LongArgument::GetName() const {
  return info_.name;
}

bool ArgumentParser::LongArgument::GetIsRequired() const {
  return info_.is_required;
}
