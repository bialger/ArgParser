#ifndef LONG_ARGUMENT_HPP_
#define LONG_ARGUMENT_HPP_

#include "Argument.h"

#include <cstdint>

namespace ArgumentParser {

class LongArgument : Argument {
 public:
  LongArgument();
  explicit LongArgument(ArgumentInformation info);
  void ValidateArgument(char* candidate, char* value, bool is_last);
  uint64_t GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;
  const char* GetName() const override;
  bool GetIsRequired() const override;

 private:
  uint64_t value_;
  ArgumentParsingStatus value_status_;
  ArgumentInformation info_;
};

} // namespace ArgumentParser

#endif //LONG_ARGUMENT_HPP_