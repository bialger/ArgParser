#ifndef TESTFUNCTIONS_HPP_
#define TESTFUNCTIONS_HPP_

#include <iostream>
#include <vector>
#include <string>

#include "lib/argparser/ConcreteArgument.hpp"

enum class Action {
  kNone = 0,
  kSum = 1,
  kMul = 2,
};

std::vector<std::string> SplitString(const std::string& str);

std::ostream& operator<<(std::ostream& os, Action action);

ArgumentParser::NonMemberParsingResult<Action> ParseAction(const std::string& action);

#endif //TESTFUNCTIONS_HPP_
