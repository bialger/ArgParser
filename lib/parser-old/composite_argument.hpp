#ifndef COMPOSITE_ARGUMENT_HPP_
#define COMPOSITE_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class CompositeArgument : Argument {
 public:
  CompositeArgument();
  explicit CompositeArgument(ArgumentInformation info);
  CompositeArgument(const CompositeArgument& other);
  CompositeArgument& operator=(const CompositeArgument& other);
  ~CompositeArgument() override;
  void ValidateArgument(char** argv, int32_t argc, char* candidate, char* value, int32_t position);
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

#endif //COMPOSITE_ARGUMENT_HPP_
