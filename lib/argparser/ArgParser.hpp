#ifndef ARGPARSER_HPP_
#define ARGPARSER_HPP_

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF) \
template<typename... Args> \
inline auto highLevelF(Args&&... args) -> decltype(lowLevelF(std::forward<Args>(args)...)) { \
    return lowLevelF(std::forward<Args>(args)...); \
}

#include <string>
#include <vector>
#include <string_view>
#include <typeinfo>
#include <map>

#include "ConcreteArgumentBuilder.hpp"
#include "ConcreteArgument.hpp"
#include "ArgParserConcepts.hpp"
#include "lib/argparser/basic/BasicFunctions.hpp"

namespace ArgumentParser {

template<ProperArgumentType ... Args>
struct ArgumentTypes {
  static std::vector<std::string_view> GetTypenames() {
    return {typeid(Args).name() ...};
  }
};

class ArgParser {
 public:
  template<ProperArgumentType ... Args>
  explicit ArgParser(const std::string_view& name = "", ArgumentTypes<Args ...> types = {});

  ArgParser(const ArgParser& other) = delete;
  ArgParser& operator=(const ArgParser& other) = delete;
  ~ArgParser();

  bool Parse(const std::vector<std::string>& args, ConditionalOutput error_output = {});
  bool Parse(int argc, char** argv, ConditionalOutput error_output = {});

  [[nodiscard]] bool Help() const;
  [[nodiscard]] std::string HelpDescription() const;

  ConcreteArgumentBuilder<bool>& AddHelp(char short_name, const std::string_view& long_name, const std::string& description = "");
  ConcreteArgumentBuilder<bool>& AddHelp(const std::string_view& long_name, const std::string& description);

  template<ProperArgumentType T>
  ConcreteArgumentBuilder<T>& AddArgument(char short_name, const std::string_view& long_name, const std::string& description = "");

  template<ProperArgumentType T>
  ConcreteArgumentBuilder<T>& AddArgument(const std::string_view& long_name, const std::string& description = "");

  template<ProperArgumentType T>
  T GetValue(const std::string_view& long_name, size_t index = 0) const;

  template<ProperArgumentType T>
  void SetAliasForType(const std::string& alias);

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
  std::string_view name_;
  std::vector<ArgumentBuilder*> argument_builders_;
  std::vector<Argument*> arguments_;
  std::vector<std::string_view> allowed_typenames_;
  std::vector<std::string> allowed_typenames_for_help_;
  std::map<std::string_view, std::map<std::string_view, size_t>> arguments_by_type_;
  std::map<char, std::string> short_to_long_names_;
  size_t help_index_;

  bool Parse_(const std::vector<std::string>& args, ConditionalOutput error_output);

  [[nodiscard]] std::vector<std::string> GetLongKeys(const std::string& current_argument) const;

  void ParsePositionalArguments(const std::vector<std::string>& argv, const std::vector<size_t>& used_positions);

  bool HandleErrors(ConditionalOutput error_output) const;

  void RefreshArguments();

  template<ProperArgumentType T>
  ConcreteArgumentBuilder<T>& AddArgument_(char short_name, const std::string_view& long_name, const std::string& description);

  template<ProperArgumentType T>
  T GetValue_(const std::string_view& long_name, size_t index) const;
};

template<ProperArgumentType... Args>
ArgumentParser::ArgParser::ArgParser(const std::string_view& name, ArgumentTypes<Args ...> types) {
  name_ = name;
  allowed_typenames_ =
      {typeid(std::string).name(), typeid(CompositeString).name(), typeid(int16_t).name(), typeid(int32_t).name(),
       typeid(int64_t).name(), typeid(uint16_t).name(), typeid(uint32_t).name(), typeid(uint64_t).name(),
       typeid(float).name(), typeid(double).name(), typeid(long double).name(), typeid(bool).name(),
       typeid(char).name()};
  allowed_typenames_for_help_ =
      {"string", "CompositeString", "short", "int", "long long", "unsigned short", "unsigned int", "unsigned long long",
       "float", "double", "long double", "bool", "char"};

  for (const std::string_view& type_name : types.GetTypenames()) {
    allowed_typenames_.emplace_back(type_name);
    allowed_typenames_for_help_.emplace_back(type_name);
  }

  argument_builders_ = {};
  arguments_ = {};
  arguments_by_type_ = {};
  short_to_long_names_ = {};

  for (const std::string_view& type_name : allowed_typenames_) {
    arguments_by_type_[type_name] = {};
  }

  help_index_ = std::string::npos;
}

template<ProperArgumentType T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument(char short_name, const std::string_view& long_name, const std::string& description) {
  return AddArgument_<T>(short_name, long_name, description);
}

template<ProperArgumentType T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument(const std::string_view& long_name, const std::string& description) {
  return AddArgument_<T>(kBadChar, long_name, description);
}

template<ProperArgumentType T>
T ArgParser::GetValue(const std::string_view& long_name, size_t index) const {
  return GetValue_<T>(long_name, index);
}

template<ProperArgumentType T>
ConcreteArgumentBuilder<T>& ArgParser::AddArgument_(char short_name, const std::string_view& long_name, const std::string& description) {
  std::map<std::string_view, size_t>& t_arguments = arguments_by_type_.at(typeid(T).name());

  if (short_name != kBadChar) {
    short_to_long_names_[short_name] = long_name;
  }

  t_arguments[long_name] = argument_builders_.size();
  auto* argument_builder = new ConcreteArgumentBuilder<T>(short_name, long_name, description);
  argument_builders_.push_back(argument_builder);

  return *argument_builder;
}

template<ProperArgumentType T>
T ArgParser::GetValue_(const std::string_view& long_name, size_t index) const {
  const std::map<std::string_view, size_t>& t_arguments = arguments_by_type_.at(typeid(T).name());
  size_t argument_index = t_arguments.at(long_name);
  auto* argument = static_cast<ConcreteArgument<T>*>(arguments_.at(argument_index));
  return argument->GetValue(index);
}

template<ProperArgumentType T>
void ArgumentParser::ArgParser::SetAliasForType(const std::string& alias) {
  auto it = std::find(allowed_typenames_.begin(), allowed_typenames_.end(), typeid(T).name());
  if (it == allowed_typenames_.end()) {
    return;
  }

  auto output_it = allowed_typenames_for_help_.begin() + (it - allowed_typenames_.begin());
  auto alias_end_it = alias.begin();

  while (alias_end_it != alias.end()) {
    if (!std::isalnum(*alias_end_it)) {
      break;
    }

    ++alias_end_it;
  }

  *output_it = std::string(alias.begin(), alias_end_it);
}

static_assert(ProperArgumentType<int8_t>);
static_assert(ProperArgumentType<int16_t>);
static_assert(ProperArgumentType<int32_t>);
static_assert(ProperArgumentType<int64_t>);
static_assert(ProperArgumentType<uint8_t>);
static_assert(ProperArgumentType<uint16_t>);
static_assert(ProperArgumentType<uint32_t>);
static_assert(ProperArgumentType<uint64_t>);
static_assert(ProperArgumentType<float>);
static_assert(ProperArgumentType<double>);
static_assert(ProperArgumentType<long double>);
static_assert(ProperArgumentType<bool>);
static_assert(ProperArgumentType<char>);
static_assert(ProperArgumentType<std::string>);
static_assert(ProperArgumentType<CompositeString>);

} // namespace ArgumentParser

#endif // ARGPARSER_HPP_
