#ifndef ARGUMENTBUILDER_H_
#define ARGUMENTBUILDER_H_

#include "Argument.h"

namespace ArgumentParser {

class ArgumentBuilder {
 public:
  virtual ~ArgumentBuilder() = default;
  virtual const ArgumentInformation& GetInfo() const = 0;
  virtual std::string GetDefaultValue() const = 0;
  virtual Argument* build() = 0;
};

} // namespace ArgumentParser

#endif //ARGUMENTBUILDER_H_
