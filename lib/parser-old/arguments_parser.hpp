#ifndef ARGUMENTS_PARSER_HPP_
#define ARGUMENTS_PARSER_HPP_

#include <cstdint>
#include <cstring>

#include "argument.hpp"
#include "bool_argument.hpp"
#include "composite_argument.hpp"
#include "long_argument.hpp"
#include "string_argument.hpp"

struct Arguments {
  BoolArgument bool_argument = BoolArgument();
  CompositeArgument composite_argument = CompositeArgument();
  LongArgument long_argument = LongArgument();
  StringArgument string_argument = StringArgument();
  ArgumentInformation info{};
};

/** \n This class represents the command-line argument parser.
 * It contains parsing methods that are independent from
 * the quantity and type of arguments. */

class ArgumentsParser {
 public:
  ArgumentsParser() = delete;
  ArgumentsParser(const ArgumentsParser& other) = delete;
  ArgumentsParser& operator=(const ArgumentsParser& other) = delete;
  ~ArgumentsParser();

  /** \n Constructor that initializes the ArgumentsParser object, which checks
   * arguments for being argument_count different values listed in arguments. */

  ArgumentsParser(ArgumentInformation* arguments, size_t argument_count);

  /** \n Parses the command-line arguments listed in arguments
   * array and returns an exit code 0 if success, 1 if failed. */

  int8_t ParseArguments(char** argv, int32_t argc);
  bool GetBoolValue(size_t index) const;
  char* GetCompositeValue(size_t index) const;
  uint64_t GetLongValue(size_t index) const;
  char* GetStringValue(size_t index) const;

 private:
  Arguments* arguments_;
  size_t size_;

  /** \n The HandleErrors function identifies and handles errors in the
   * ParsingResult by outputting error messages and setting an exit code. It
   * ensures that any errors or inconsistencies in the command-line arguments
   * are appropriately addressed and reported to the user. */

  int8_t HandleErrors();
};

#endif // ARGUMENTS_PARSER_HPP_
