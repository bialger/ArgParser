#include <iterator>
#include <sstream>

#include "test_functions.hpp"

std::vector<std::string> SplitString(const std::string& str) {
  std::istringstream iss(str);

  return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

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

ArgumentParser::NonMemberParsingResult<Action> ParseAction(const std::string& action) {
  ArgumentParser::NonMemberParsingResult<Action> result{};

  if (action == "sum") {
    result.value = Action::kSum;
  } else if (action == "mul") {
    result.value = Action::kMul;
  } else {
    result.success = false;
    result.value = Action::kNone;
  }

  return result;
}

AddArgumentType(Action, ParseAction)
