#ifndef CONDITIONALOUTPUT_HPP_
#define CONDITIONALOUTPUT_HPP_

#include <iostream>

/**\n Structure for error output in ArgumentParser */

namespace ArgumentParser {

struct ConditionalOutput {
  std::ostream& out_stream = std::cout;
  bool print_messages = false;

  template<typename T>
  requires requires(T t, std::ostream& os) {
    { os << t } -> std::same_as<std::ostream&>;
  }
  ConditionalOutput& operator<<(const T& t) {
    if (print_messages) {
      out_stream << t;
    }

    return *this;
  }
};

}

#endif //CONDITIONALOUTPUT_HPP_
