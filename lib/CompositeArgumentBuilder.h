#ifndef COMPOSITEARGUMENTBUILDER_H_
#define COMPOSITEARGUMENTBUILDER_H_

#include <string>
#include <vector>

#include "ArgumentBuilder.h"

namespace ArgumentParser {

class CompositeArgumentBuilder : public ArgumentBuilder {
 public:
  CompositeArgumentBuilder();

  CompositeArgumentBuilder& MultiValue(int min);
  CompositeArgumentBuilder& MultiValue();
  CompositeArgumentBuilder& Positional();
  CompositeArgumentBuilder& StoreValue(const std::string& value);
  CompositeArgumentBuilder& StoreValues(const std::vector<std::string>& values);
  CompositeArgumentBuilder& Default(const std::string& value);

  void build();
};

} // namespace ArgumentParser

#endif //COMPOSITEARGUMENTBUILDER_H_
