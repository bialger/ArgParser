#ifndef ARGUMENT_HPP_
#define ARGUMENT_HPP_

enum class ArgumentParsingStatus {
  kNoArgument,
  kBrokenArgument,
  kSuccess
};

enum class ArgumentType {
  kBoolArgument,
  kCompositeArgument,
  kLongArgument,
  kStringArgument
};

/** \n This structure represents a single information.
 * Contains all required parameters for all argument types. */

struct ArgumentInformation {
  const char* short_key = nullptr;
  const char* long_key = nullptr;
  const char* name = nullptr;
  ArgumentType type = ArgumentType::kBoolArgument;
  bool is_required = false;
  bool (* Validate)(char*) = nullptr;
  bool (* IsGood)(char*) = nullptr;
};

class Argument {
 public:
  virtual ~Argument() = default;
  virtual ArgumentParsingStatus GetValueStatus() const = 0;
  virtual ArgumentType GetType() const = 0;
  virtual const char* GetName() const = 0;
  virtual bool GetIsRequired() const = 0;
};

#endif //ARGUMENT_HPP_
