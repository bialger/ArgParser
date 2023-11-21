#ifndef ARGUMENTBUILDER_H_
#define ARGUMENTBUILDER_H_

namespace ArgumentParser {

class ArgumentBuilder {
 public:
   virtual ~ArgumentBuilder() = 0;

   virtual void build() = 0;
};

} // namespace ArgumentParser

#endif //ARGUMENTBUILDER_H_
