#ifndef BOOL_ARGUMENT_HPP_
#define BOOL_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class BoolArgument : Argument {
 public:
  BoolArgument();
  explicit BoolArgument(ArgumentInformation info);
  void ValidateArgument(char* candidate);
  bool GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;
  const char* GetName() const override;
  bool GetIsRequired() const override;

 private:
  bool value_;
  ArgumentParsingStatus value_status_;
  ArgumentInformation info_;
};

#endif //BOOL_ARGUMENT_HPP_
