#ifndef ARGUMENT_H_
#define ARGUMENT_H_

#include <string>
#include <vector>

#include "lib/argparser/basic/BasicFunctions.hpp"

class CompositeString : public std::string {
 private:
  using base = std::string;
 public:
  using base::base;
};

namespace ArgumentParser {

constexpr char kBadChar = '\x15';

enum class ArgumentParsingStatus {
  kNoArgument,
  kInvalidArgument,
  kInsufficientArguments,
  kSuccess
};

/** \n This structure represents a single information.
 * Contains all required parameters for all argument types. */

struct ArgumentInformation {
  char short_key = kBadChar;
  const char* long_key = "";
  const char* description = "";
  std::string type;
  size_t minimum_values = 0;
  bool is_multi_value = false;
  bool is_positional = false;
  bool has_store_values = false;
  bool has_store_value = false;
  bool has_default = false;
  bool (* Validate)(std::string&) = &AlwaysTrue;
  bool (* IsGood)(std::string&) = &AlwaysTrue;
};

class Argument {
 public:
  virtual std::vector<size_t> ValidateArgument(const std::vector<std::string>& argv, size_t position) = 0;
  virtual bool CheckLimit() = 0;
  virtual ArgumentParsingStatus GetValueStatus() const = 0;
  virtual const std::string& GetType() const = 0;
  virtual const ArgumentInformation& GetInfo() const = 0;
  virtual size_t GetUsedValues() const = 0;
  virtual void ClearStored() = 0;
 protected:
  virtual size_t ObtainValue(const std::vector<std::string>& argv, std::string& value_string,
                             std::vector<size_t>& used_values, size_t position) = 0;
};

} // namespace ArgumentParser

#endif //ARGUMENT_H_