#ifndef LONGARGUMENTBUILDER_H_
#define LONGARGUMENTBUILDER_H_

#include <cstdint>
#include <vector>

#include "ArgumentBuilder.h"

namespace ArgumentParser {

class LongArgumentBuilder : public ArgumentBuilder {
 public:
  LongArgumentBuilder();

  LongArgumentBuilder& MultiValue(int min);
  LongArgumentBuilder& MultiValue();
  LongArgumentBuilder& Positional();
  LongArgumentBuilder& StoreValue(const int64_t& value);
  LongArgumentBuilder& StoreValues(const std::vector<int32_t>& values);
  LongArgumentBuilder& Default(int64_t value);

  void build();
};

} // namespace ArgumentParser

#endif //LONGARGUMENTBUILDER_H_
