#ifndef STRING_ARGUMENT_HPP_
#define STRING_ARGUMENT_HPP_

#include "Argument.h"

#include <cstdint>

namespace ArgumentParser {

class StringArgument : Argument {
 public:
  StringArgument();
  explicit StringArgument(ArgumentInformation info);
  StringArgument(const StringArgument& other);
  StringArgument& operator=(const StringArgument& other);
  ~StringArgument() override;
  void ValidateArgument(char* candidate, char* value, bool is_last);
  char* GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;
  const char* GetName() const override;
  bool GetIsRequired() const override;

 private:
  constexpr static char* const kError = nullptr;
  char* value_;
  ArgumentParsingStatus value_status_;
  ArgumentInformation info_;
};

} // namespace ArgumentParser

#endif //STRING_ARGUMENT_HPP_
