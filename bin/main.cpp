#include <functional>
#include <iostream>
#include <numeric>

#include "lib/argparser/ArgParser.hpp"

enum class Action {
  kNone = 0,
  kSum = 1,
  kMul = 2,
};

std::ostream& operator<<(std::ostream& os, Action action) {
  switch (action) {
    case Action::kSum:
      os << "sum";
      break;
    case Action::kMul:
      os << "mul";
      break;
    case Action::kNone:
      os << "none";
      break;
  }

  return os;
}

Action ParseOneAction(const std::string& action) {
  if (action == "sum") {
    return Action::kSum;
  } else if (action == "mul") {
    return Action::kMul;
  } else {
    return Action::kNone;
  }
}

ArgumentParser::NonMemberParsingResult<Action> ParseAction(const std::vector<std::string>& argv,
                   std::string& value_string,
                   std::vector<size_t>& used_values,
                   size_t position) {
  Action pre_value = ParseOneAction(value_string);
  ArgumentParser::NonMemberParsingResult<Action> result{};
  result.position = position;

  if (pre_value == Action::kNone) {
    result.success = false;
  } else {
    result.result = pre_value;
  }

  return result;
}

AddArgumentType(Action, ParseAction);

int main(int argc, char** argv) {
  std::vector<int> values;
  Action action{};

  ArgumentParser::ArgParser parser("Program", PassArgumentTypes(Action));
  parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
  parser.AddArgument<Action>('a', "action", "Action type").StoreValue(action);
  parser.AddHelp('h', "help", "Program accumulate arguments");

  if (!parser.Parse(argc, argv, {std::cout, true})) {
    std::cout << "Wrong argument" << std::endl;
    std::cout << parser.HelpDescription() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.HelpDescription() << std::endl;
    return 0;
  }

  switch (action) {
    case Action::kSum:
      std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
      break;
    case Action::kMul:
      std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<>()) << std::endl;
      break;
    case Action::kNone:
      std::cout << "No one options had chosen" << std::endl;
      std::cout << parser.HelpDescription();
      return 1;
  }

  return 0;

}
