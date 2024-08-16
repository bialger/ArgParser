#ifndef ARGPARSER_HPP_
#define ARGPARSER_HPP_

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF) \
template<typename... Args> \
inline auto highLevelF(Args&&... args) -> decltype(lowLevelF(std::forward<Args>(args)...)) { \
    return lowLevelF(std::forward<Args>(args)...); \
}

#include <string>
#include <typeinfo>
#include <map>

#include "ConcreteArgumentBuilder.hpp"
#include "ConcreteArgument.hpp"
#include "lib/argparser/basic/BasicFunctions.hpp"

namespace ArgumentParser {

template<typename ... Args>
struct ArgumentTypes {
  static std::vector<std::string> GetTypenames() {
    return {typeid(Args).name() ...};
  }
};

class ArgParser {
 public:
  template<typename ... Args>
  ArgParser(const char* name = "", ArgumentTypes<Args ...> types = {});

  ArgParser(const ArgParser& other) = delete;
  ArgParser& operator=(const ArgParser& other) = delete;
  ~ArgParser();

  bool Parse(const std::vector<std::string>& args, ConditionalOutput error_output = {});
  bool Parse(int argc, char** argv, ConditionalOutput error_output = {});

  bool Help();
  std::string HelpDescription();

  ConcreteArgumentBuilder<bool>& AddHelp(char short_name, const char* long_name, const char* description = "");
  ConcreteArgumentBuilder<bool>& AddHelp(const char* long_name, const char* description);

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument(char short_name, const char* long_name, const char* description = "");

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument(const char* long_name, const char* description = "");

  template<typename T>
  T GetValue(const char* long_name, size_t index = 0);

  ALIAS_TEMPLATE_FUNCTION(AddShortArgument, AddArgument<int16_t>);
  ALIAS_TEMPLATE_FUNCTION(AddIntArgument, AddArgument<int32_t>);
  ALIAS_TEMPLATE_FUNCTION(AddLongLongArgument, AddArgument<int64_t>);
  ALIAS_TEMPLATE_FUNCTION(AddUnsignedShortArgument, AddArgument<uint16_t>);
  ALIAS_TEMPLATE_FUNCTION(AddUnsignedIntArgument, AddArgument<uint32_t>);
  ALIAS_TEMPLATE_FUNCTION(AddUnsignedLongLongArgument, AddArgument<uint64_t>);
  ALIAS_TEMPLATE_FUNCTION(AddFloatArgument, AddArgument<float>);
  ALIAS_TEMPLATE_FUNCTION(AddDoubleArgument, AddArgument<double>);
  ALIAS_TEMPLATE_FUNCTION(AddLongDoubleArgument, AddArgument<long double>);
  ALIAS_TEMPLATE_FUNCTION(AddFlag, AddArgument<bool>);
  ALIAS_TEMPLATE_FUNCTION(AddCharArgument, AddArgument<char>);
  ALIAS_TEMPLATE_FUNCTION(AddStringArgument, AddArgument<std::string>);
  ALIAS_TEMPLATE_FUNCTION(AddCompositeArgument, AddArgument<CompositeString>);

  ALIAS_TEMPLATE_FUNCTION(GetShortValue, GetValue<int16_t>);
  ALIAS_TEMPLATE_FUNCTION(GetIntValue, GetValue<int32_t>);
  ALIAS_TEMPLATE_FUNCTION(GetLongLongValue, GetValue<int64_t>);
  ALIAS_TEMPLATE_FUNCTION(GetUnsignedShortValue, GetValue<uint16_t>);
  ALIAS_TEMPLATE_FUNCTION(GetUnsignedIntValue, GetValue<uint32_t>);
  ALIAS_TEMPLATE_FUNCTION(GetUnsignedLongLongValue, GetValue<uint64_t>);
  ALIAS_TEMPLATE_FUNCTION(GetFloatValue, GetValue<float>);
  ALIAS_TEMPLATE_FUNCTION(GetDoubleValue, GetValue<double>);
  ALIAS_TEMPLATE_FUNCTION(GetLongDoubleValue, GetValue<long double>);
  ALIAS_TEMPLATE_FUNCTION(GetFlag, GetValue<bool>);
  ALIAS_TEMPLATE_FUNCTION(GetChar, GetValue<char>);
  ALIAS_TEMPLATE_FUNCTION(GetStringValue, GetValue<std::string>);
  ALIAS_TEMPLATE_FUNCTION(GetCompositeValue, GetValue<CompositeString>);

 private:
  const char* name_;
  std::vector<ArgumentBuilder*> argument_builders_;
  std::vector<Argument*> arguments_;
  std::vector<std::string> allowed_typenames_;
  std::vector<std::string> allowed_typenames_for_help_;
  std::map<std::string, std::map<std::string, size_t>> arguments_by_type_;
  std::map<char, std::string> short_to_long_names_;
  size_t help_index_;

  bool Parse_(const std::vector<std::string>& args, ConditionalOutput error_output);

  std::vector<std::string> GetLongKeys(const std::string& current_argument);

  void ParsePositionalArguments(const std::vector<std::string>& argv, const std::vector<size_t>& used_positions);

  bool HandleErrors(ConditionalOutput error_output);

  void RefreshArguments();

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument_(char short_name, const char* long_name, const char* description);

  template<typename T>
  T GetValue_(const char* long_name, size_t index);
};

template<typename... Args>
ArgumentParser::ArgParser::ArgParser(const char* name, ArgumentTypes<Args ...> types) {
  name_ = name;
  allowed_typenames_ =
      {typeid(std::string).name(), typeid(CompositeString).name(), typeid(int16_t).name(), typeid(int32_t).name(),
       typeid(int64_t).name(), typeid(uint16_t).name(), typeid(uint32_t).name(), typeid(uint64_t).name(),
       typeid(float).name(), typeid(double).name(), typeid(long double).name(), typeid(bool).name(),
       typeid(char).name()};
  allowed_typenames_for_help_ =
      {"string", "CompositeString", "short", "int", "long long", "unsigned short", "unsigned int", "unsigned long long",
       "float", "double", "long double", "bool", "char"};

  for (const std::string& type_name : types.GetTypenames()) {
    allowed_typenames_.emplace_back(type_name);
    allowed_typenames_for_help_.emplace_back(type_name);
  }

  argument_builders_ = {};
  arguments_ = {};
  arguments_by_type_ = {};
  short_to_long_names_ = {};

  for (const std::string& type_name : allowed_typenames_) {
    arguments_by_type_[type_name] = {};
  }

  help_index_ = std::string::npos;
}

template<typename T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument(char short_name, const char* long_name, const char* description) {
  return AddArgument_<T>(short_name, long_name, description);
}

template<typename T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument(const char* long_name, const char* description) {
  return AddArgument_<T>(kBadChar, long_name, description);
}

template<typename T>
T ArgParser::GetValue(const char* long_name, size_t index) {
  return GetValue_<T>(long_name, index);
}

template<typename T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument_(char short_name, const char* long_name, const char* description) {
  std::map<std::string, size_t>* t_arguments = &arguments_by_type_.at(typeid(T).name());

  if (short_name != kBadChar) {
    short_to_long_names_[short_name] = long_name;
  }

  (*t_arguments)[long_name] = argument_builders_.size();
  auto* argument_builder = new ConcreteArgumentBuilder<T>(short_name, long_name, description);
  argument_builders_.push_back(argument_builder);

  return *argument_builder;
}

template<typename T>
T ArgParser::GetValue_(const char* long_name, size_t index) {
  std::map<std::string, size_t>* t_arguments = &arguments_by_type_.at(typeid(T).name());
  size_t argument_index = t_arguments->at(long_name);
  auto* argument = static_cast<ConcreteArgument<T>*>(arguments_.at(argument_index));
  return argument->GetValue(index);
}

} // namespace ArgumentParser

#endif // ARGPARSER_HPP_
