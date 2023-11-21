#ifndef BOOLARGUMENTBUILDER_H_
#define BOOLARGUMENTBUILDER_H_

#include <vector>

#include "ArgumentBuilder.h"

namespace ArgumentParser {

class BoolArgumentBuilder : public ArgumentBuilder {
 public:
  BoolArgumentBuilder();

  BoolArgumentBuilder& MultiValue(int min);
  BoolArgumentBuilder& MultiValue();
  BoolArgumentBuilder& Positional();
  BoolArgumentBuilder& StoreValue(const bool& value);
  BoolArgumentBuilder& StoreValues(const std::vector<bool>& values);
  BoolArgumentBuilder& Default(bool value);

  void build();
};

} // namespace ArgumentParser

#endif //BOOLARGUMENTBUILDER_H_
