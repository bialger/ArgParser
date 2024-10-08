#include "ArgParser.hpp"

ArgumentParser::ArgParser::~ArgParser() {
  for (ArgumentBuilder*& argument_builder : argument_builders_) {
    delete argument_builder;
  }

  for (Argument*& argument : arguments_) {
    delete argument;
  }
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& args, ConditionalOutput error_output) {
  return Parse_(args, error_output);
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv, ConditionalOutput error_output) {
  const auto args = std::vector<std::string>(argv, argv + argc);

  return Parse_(args, error_output);
}

bool ArgumentParser::ArgParser::Help() const {
  if (help_index_ == std::string::npos) {
    return false;
  }

  const auto* help_argument = dynamic_cast<ConcreteArgument<bool>*>(arguments_[help_index_]);

  return help_argument->GetValue(0);
}

std::string ArgumentParser::ArgParser::HelpDescription() const {
  if (help_index_ == std::string::npos) {
    return {};
  }

  std::string help;
  help += name_;
  help += "\n";
  help += argument_builders_[help_index_]->GetInfo().description;
  help += "\n\nOPTIONS:\n";

  for (size_t i = 0; i < allowed_typenames_.size(); ++i) {
    std::string_view type_name = allowed_typenames_[i];
    std::string output_type_name = allowed_typenames_for_help_[i];

    for (const auto& index : arguments_by_type_.at(type_name) | std::views::values) {
      if (index == help_index_) {
        continue;
      }

      ArgumentBuilder* argument = argument_builders_[index];
      auto [short_key, long_key, description, type, minimum_values, is_multi_value, is_positional, has_store_values,
        has_store_value, has_default, validate, is_good] = argument->GetInfo();
      help += short_key == kBadChar ? "     " : std::string("-") + short_key + ",  ";
      help += "--";
      help += long_key;

      if (type_name != typeid(bool).name()) {
        help += "=<" + output_type_name + ">";
      }

      help += ":  ";
      help += description;

      help += " [";
      if (is_multi_value) {
        help += "repeated, ";
      }

      if (is_positional) {
        help += "positional, ";
      }

      if (has_default && (type_name != typeid(bool).name() || argument->GetDefaultValue() != "0")) {
        help += "default = ";

        if (type_name == typeid(bool).name()) {
          help += (argument->GetDefaultValue() == "0") ? "false" : "true";
        } else {
          help += argument->GetDefaultValue();
        }

        help += ", ";
      }

      if (minimum_values != 0 && is_multi_value) {
        help += "min args = " + std::to_string(minimum_values);
      }

      bool was_extended = help.back() != '[';

      if (!was_extended || help.back() == ' ') {
        help = help.substr(0, help.size() - 2);
      }

      if (was_extended) {
        help += "]";
      }

      help += "\n";
    }
  }

  help += "\n";
  ArgumentBuilder* argument = argument_builders_[help_index_];
  auto [short_key, long_key, description, type, minimum_values, is_multi_value, is_positional, has_store_values,
    has_store_value, has_default, validate, is_good] = argument->GetInfo();
  help += short_key == kBadChar ? "     " : std::string("-") + short_key + ",  ";
  help += "--";
  help += long_key;
  help += ":  Display this help and exit";
  help += "\n";

  return help;
}

ArgumentParser::ConcreteArgumentBuilder<bool>& ArgumentParser::ArgParser::AddHelp(char short_name,
                                                                                  const std::string_view& long_name,
                                                                                  const std::string& description) {
  ConcreteArgumentBuilder<bool>* help_argument_ = &AddArgument<bool>(short_name, long_name, description);
  help_index_ = argument_builders_.size() - 1;
  return *help_argument_;
}

ArgumentParser::ConcreteArgumentBuilder<bool>& ArgumentParser::ArgParser::AddHelp(const std::string_view& long_name,
  const std::string& description) {
  return AddHelp(kBadChar, long_name, description);
}

bool ArgumentParser::ArgParser::Parse_(const std::vector<std::string>& args, ConditionalOutput error_output) {
  RefreshArguments();
  std::vector<size_t> used_positions = {0};
  std::vector<std::string> argv = args;

  for (std::string& arg : argv) {
    if (arg[0] == '\'' || arg[0] == '"') {
      arg = arg.substr(1);
    }

    if (arg.back() == '\'' || arg.back() == '"') {
      arg = arg.substr(0, arg.size() - 1);
    }
  }

  argv.emplace_back("--");

  for (size_t position = 1; position < argv.size() && argv[position] != "--"; ++position) {
    if (argv[position][0] == '-') {
      if (argv[position].size() == 1) {
        return false;
      }

      std::vector<std::string_view> long_keys = GetLongKeys(argv[position]);

      if (long_keys.empty()) {
        DisplayError("Used nonexistent argument: " + argv[position] + "\n", error_output);
        return false;
      }

      for (const std::string_view& long_key : long_keys) {
        bool was_found = false;

        for (const std::string_view& type_name : allowed_typenames_) {
          const std::map<std::string_view, size_t>* t_arguments = &arguments_by_type_.at(type_name);

          if (t_arguments->contains(long_key)) {
            was_found = true;
            std::vector<size_t> current_used_positions =
                arguments_[t_arguments->at(long_key)]->ValidateArgument(argv, position);
            position = (current_used_positions.empty()) ? position : current_used_positions.back();
            used_positions.insert(std::end(used_positions),
                                  std::begin(current_used_positions),
                                  std::end(current_used_positions));
          }

          if (was_found) {
            break;
          }
        }

        if (!was_found) {
          DisplayError("Used nonexistent argument: " + argv[position] + "\n", error_output);
          return false;
        }
      }
    }
  }

  ParsePositionalArguments(argv, used_positions);

  return HandleErrors(error_output);
}

std::vector<std::string_view> ArgumentParser::ArgParser::GetLongKeys(const std::string_view& current_argument) const {
  std::string_view one_long_key = current_argument.substr(2);
  std::string_view result_key;

  if (one_long_key.find('=') != std::string::npos) {
    result_key = one_long_key.substr(0, one_long_key.find('='));
  } else {
    result_key = one_long_key;
  }

  std::vector<std::string_view> long_keys;

  if (current_argument[1] != '-') {
    for (size_t current_key_index = 1;
         current_key_index < current_argument.size() &&
         short_to_long_names_.contains(current_argument[current_key_index]);
         ++current_key_index) {
      long_keys.emplace_back(short_to_long_names_.at(current_argument[current_key_index]));
    }
  } else {
    long_keys.push_back(result_key);
  }

  return long_keys;
}

void ArgumentParser::ArgParser::ParsePositionalArguments(const std::vector<std::string>& argv,
                                                         const std::vector<size_t>& used_positions) const {
  std::vector<std::string> positional_args = {};
  std::vector<size_t> positional_indices = {};

  for (size_t i = 0; i < arguments_.size(); ++i) {
    if (arguments_[i]->GetInfo().is_positional) {
      positional_indices.push_back(i);
    }
  }

  for (size_t i = 0; i < argv.size(); ++i) {
    if (std::ranges::find(used_positions, i) == std::end(used_positions)) {
      positional_args.push_back(argv[i]);
    }
  }

  for (size_t position = 0, argument_index = 0;
       position < positional_args.size() &&
       argument_index < positional_indices.size() &&
       positional_args[position] != "--";
       ++position, ++argument_index) {
    std::vector<size_t> current_used_positions =
        arguments_[positional_indices[argument_index]]->ValidateArgument(positional_args, position);
    position = (current_used_positions.empty()) ? position : current_used_positions.back();
  }
}

void ArgumentParser::ArgParser::RefreshArguments() {
  for (Argument* argument : arguments_) {
    argument->ClearStored();
    delete argument;
  }

  arguments_.clear();

  for (ArgumentBuilder* argument_builder : argument_builders_) {
    arguments_.push_back(argument_builder->build());
  }
}

bool ArgumentParser::ArgParser::HandleErrors(ConditionalOutput error_output) const {
  std::string error_string;
  bool is_correct = true;

  if (help_index_ != std::string::npos && Help()) {
    return true;
  }

  for (const auto& argument : arguments_) {
    if (!argument->CheckLimit()) {
      error_string += "Not enough values were passed to argument --";
      error_string += argument->GetInfo().long_key;
      error_string += ".\n";
      is_correct = false;
    }

    if (argument->GetValueStatus() == ArgumentParsingStatus::kInvalidArgument) {
      error_string += "An incorrect value was passed to the --";
      error_string += argument->GetInfo().long_key;
      error_string += " argument.\n";
      is_correct = false;
    }
  }

  DisplayError(error_string, error_output);

  return is_correct;
}
