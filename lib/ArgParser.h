#ifndef ARGPARSER_H_
#define ARGPARSER_H_

#include <string>

#include "ArgumentBuilder.h"
#include "BoolArgumentBuilder.h"
#include "StringArgumentBuilder.h"
#include "LongArgumentBuilder.h"
#include "CompositeArgumentBuilder.h"

namespace ArgumentParser {

class ArgParser {
 public:
  ArgParser();
  ArgParser(const char* name);
  ArgParser(const ArgParser& other) = delete;
  ArgParser& operator=(const ArgParser& other) = delete;
  ~ArgParser();

  bool Parse(const std::vector<std::string>& args);
  bool Parse(int argc, char** argv);

  bool Help();
  std::string& HelpDescription();

  BoolArgumentBuilder& AddFlag(char short_name, const char* long_name, const char* description);
  BoolArgumentBuilder& AddFlag(char short_name, const char* long_name);
  BoolArgumentBuilder& AddFlag(const char* long_name, const char* description);
  BoolArgumentBuilder& AddFlag(const char* long_name);

  StringArgumentBuilder& AddStringArgument(char short_name, const char* long_name, const char* description);
  StringArgumentBuilder& AddStringArgument(char short_name, const char* long_name);
  StringArgumentBuilder& AddStringArgument(const char* long_name, const char* description);
  StringArgumentBuilder& AddStringArgument(const char* long_name);

  LongArgumentBuilder& AddIntArgument(char short_name, const char* long_name, const char* description);
  LongArgumentBuilder& AddIntArgument(char short_name, const char* long_name);
  LongArgumentBuilder& AddIntArgument(const char* long_name, const char* description);
  LongArgumentBuilder& AddIntArgument(const char* long_name);

  CompositeArgumentBuilder& AddCompositeArgument(char short_name, const char* long_name, const char* description);
  CompositeArgumentBuilder& AddCompositeArgument(char short_name, const char* long_name);
  CompositeArgumentBuilder& AddCompositeArgument(const char* long_name, const char* description);
  CompositeArgumentBuilder& AddCompositeArgument(const char* long_name);

  BoolArgumentBuilder& AddHelp(char short_name, const char* long_name, const char* description);
  BoolArgumentBuilder& AddHelp(char short_name, const char* long_name);
  BoolArgumentBuilder& AddHelp(const char* long_name, const char* description);
  BoolArgumentBuilder& AddHelp(const char* long_name);

  bool GetFlag(const char* long_name);
  bool GetFlag(const char* long_name, size_t index);
  int64_t GetIntValue(const char* long_name);
  int64_t GetIntValue(const char* long_name, size_t index);
  std::string GetStringValue(const char* long_name, size_t index);
  std::string GetStringValue(const char* long_name);
 private:
  const char* name_;
  std::vector<ArgumentBuilder*> arguments_;

};

} // namespace ArgumentParser

#endif // ARGPARSER_H_
