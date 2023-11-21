#ifndef ARGPARSER_H_
#define ARGPARSER_H_

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF) \
template<typename... Args> \
inline auto highLevelF(Args&&... args) -> decltype(lowLevelF(std::forward<Args>(args)...)) \
{ \
    return lowLevelF(std::forward<Args>(args)...); \
}

#include <string>
#include <typeinfo>
#include <map>

#include "ConcreteArgumentBuilder.hpp"
#include "ConcreteArgument.hpp"

namespace ArgumentParser {

class ArgParser {
 public:
  explicit ArgParser(const char* name = "");
  ArgParser(const ArgParser& other) = delete;
  ArgParser& operator=(const ArgParser& other) = delete;
  ~ArgParser();

  bool Parse(const std::vector<std::string>& args, bool print_errors = false);
  bool Parse(int argc, char** argv, bool print_errors = false);

  bool Help();
  std::string HelpDescription();

  ConcreteArgumentBuilder<bool>& AddHelp(char short_name, const char* long_name, const char* description = "");
  ConcreteArgumentBuilder<bool>& AddHelp(const char* long_name, const char* description);

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument(char short_name, const char* long_name, const char* description = "") {
    return AddArgument_<T>(short_name, long_name, description);
  }

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument(const char* long_name, const char* description = "") {
    return AddArgument_<T>(kBadChar, long_name, description);
  }

  template<typename T>
  T GetValue(const char* long_name, size_t index = 0) {
    return GetValue_<T>(long_name, index);
  }

  ALIAS_TEMPLATE_FUNCTION(AddFlag, AddArgument<bool>);
  ALIAS_TEMPLATE_FUNCTION(AddStringArgument, AddArgument<std::string>);
  ALIAS_TEMPLATE_FUNCTION(AddIntArgument, AddArgument<int32_t>);
  ALIAS_TEMPLATE_FUNCTION(AddCompositeArgument, AddArgument<CompositeString>);

  ALIAS_TEMPLATE_FUNCTION(GetFlag, GetValue<bool>);
  ALIAS_TEMPLATE_FUNCTION(GetIntValue, GetValue<int32_t>);
  ALIAS_TEMPLATE_FUNCTION(GetStringValue, GetValue<std::string>);
  ALIAS_TEMPLATE_FUNCTION(GetCompositeValue, GetValue<CompositeString>);
 private:
  const char* name_;
  std::vector<ArgumentBuilder*> argument_builders_;
  std::vector<Argument*> arguments_;
  std::vector<std::string> allowed_typenames_;
  std::map<std::string, std::map<std::string, size_t>> arguments_by_type_;
  std::map<char, std::string> short_to_long_names_;
  size_t help_index_;

  bool Parse_(const std::vector<std::string>& args, bool print_errors);

  bool HandleErrors(bool print_errors);

  void RefreshArguments();

  template<typename T>
  ConcreteArgumentBuilder<T>& AddArgument_(char short_name, const char* long_name, const char* description) {
    std::map<std::string, size_t>* t_arguments = &arguments_by_type_.at(typeid(T).name());

    if (short_name != kBadChar) {
      short_to_long_names_[short_name] = long_name;
    }

    (*t_arguments)[long_name] = argument_builders_.size();
    ConcreteArgumentBuilder<T>* argument_builder = new ConcreteArgumentBuilder<T>(short_name, long_name, description);
    argument_builders_.push_back(argument_builder);

    return *argument_builder;
  }

  template<typename T>
  T GetValue_(const char* long_name, size_t index) {
    if (arguments_.size() < argument_builders_.size()) {
      throw std::runtime_error("Argument " + std::string(long_name) + " is not ready yet.");
    }

    std::map<std::string, size_t>* t_arguments = &arguments_by_type_.at(typeid(T).name());
    size_t argument_index = t_arguments->at(long_name);
    ConcreteArgument<T>* argument = static_cast<ConcreteArgument<T>*>(arguments_[argument_index]);
    return argument->GetValue(index);
  }
};

} // namespace ArgumentParser

#endif // ARGPARSER_H_
