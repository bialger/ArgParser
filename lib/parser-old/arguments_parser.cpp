#include "arguments_parser.hpp"
#include "lib/basic/basic_functions.hpp"

#include <iostream>

ArgumentsParser::ArgumentsParser(ArgumentInformation* arguments, size_t argument_count) {
  size_ = argument_count;
  arguments_ = new Arguments[size_];

  for (size_t i = 0; i < size_; ++i) {
    arguments_[i] = Arguments{};
    arguments_[i].info = arguments[i];

    switch (arguments[i].type) {
      case ArgumentType::kBoolArgument:
        arguments_[i].bool_argument = BoolArgument(arguments[i]);
        break;
      case ArgumentType::kCompositeArgument:
        arguments_[i].composite_argument = CompositeArgument(arguments[i]);
        break;
      case ArgumentType::kLongArgument:
        arguments_[i].long_argument = LongArgument(arguments[i]);
        break;
      case ArgumentType::kStringArgument:
        arguments_[i].string_argument = StringArgument(arguments[i]);
        break;
    }
  }
}


ArgumentsParser::~ArgumentsParser() {
  delete[] arguments_;
}

int8_t ArgumentsParser::ParseArguments(char** argv, int32_t argc) {
  for (int32_t i = 1; i < argc; ++i) {
    char* candidate = argv[i];
    char* value = nullptr;
    bool is_last = (i == argc - 1);

    if (!is_last) {
      value = argv[i + 1];
    }

    for (size_t j = 0; j < size_; ++j) {
      switch (arguments_[j].info.type) {
        case ArgumentType::kBoolArgument:
          arguments_[j].bool_argument.ValidateArgument(candidate);
          break;
        case ArgumentType::kCompositeArgument:
          arguments_[j].composite_argument.ValidateArgument(argv, argc, candidate, value, i);
          break;
        case ArgumentType::kLongArgument:
          arguments_[j].long_argument.ValidateArgument(candidate, value, is_last);
          break;
        case ArgumentType::kStringArgument:
          arguments_[j].string_argument.ValidateArgument(candidate, value, is_last);
          break;
      }
    }
  }

  return HandleErrors();
}

int8_t ArgumentsParser::HandleErrors() {
  int8_t exitCode = 0;
  std::cout << (IsWindows() ? "" : "\x1B[31m");

  /* Changes the color of the console output to red if not running
   * on Windows. */

  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
  }

  /* Conditional block to set the console text color to red if the program is
   * running on Windows, using the Windows API functions. */

  for (size_t i = 0; i < size_; ++i) {
    ArgumentInformation current_argument_information = arguments_[i].info;
    ArgumentParsingStatus current_status = ArgumentParsingStatus::kBrokenArgument;

    switch (arguments_[i].info.type) {
      case ArgumentType::kBoolArgument:
        current_status = arguments_[i].bool_argument.GetValueStatus();
        break;
      case ArgumentType::kCompositeArgument:
        current_status = arguments_[i].composite_argument.GetValueStatus();
        break;
      case ArgumentType::kLongArgument:
        current_status = arguments_[i].long_argument.GetValueStatus();
        break;
      case ArgumentType::kStringArgument:
        current_status = arguments_[i].string_argument.GetValueStatus();
        break;
    }

    if (current_argument_information.is_required && current_status == ArgumentParsingStatus::kNoArgument) {
      std::cout << "Error! " << current_argument_information.name << " not specified. \n\n";
      exitCode = 1;
    } else if (current_status == ArgumentParsingStatus::kBrokenArgument) {
      std::cout << "Error! " << current_argument_information.name << " is incorrect. \n\n";
      exitCode = 1;
    }
  }

  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
  }

  std::cout << (IsWindows() ? "" : "\x1B[0m");
  return exitCode;
}

bool ArgumentsParser::GetBoolValue(size_t index) const {
  return arguments_[index].bool_argument.GetValue();
}

char* ArgumentsParser::GetCompositeValue(size_t index) const {
  return arguments_[index].composite_argument.GetValue();
}

uint64_t ArgumentsParser::GetLongValue(size_t index) const {
  return arguments_[index].long_argument.GetValue();
}

char* ArgumentsParser::GetStringValue(size_t index) const {
  return arguments_[index].string_argument.GetValue();
}
