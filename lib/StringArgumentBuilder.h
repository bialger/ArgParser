#ifndef STRINGARGUMENTBUILDER_H_
#define STRINGARGUMENTBUILDER_H_

#include <string>
#include <vector>

#include "ArgumentBuilder.h"

namespace ArgumentParser {

class StringArgumentBuilder : public ArgumentBuilder {
 public:
  StringArgumentBuilder();

  StringArgumentBuilder& MultiValue(int min);
  StringArgumentBuilder& MultiValue();
  StringArgumentBuilder& Positional();
  StringArgumentBuilder& StoreValue(const std::string& value);
  StringArgumentBuilder& StoreValues(const std::vector<std::string>& values);
  StringArgumentBuilder& Default(const std::string& value);

  void build();
};

} // namespace ArgumentParser

#endif //LABWORK4_LIB_STRINGARGUMENTBUILDER_H_
