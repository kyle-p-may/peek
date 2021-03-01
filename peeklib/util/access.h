#ifndef PEEKLIB_UTIL_BUILDER_H
#define PEEKLIB_UTIL_BUILDER_H

#include <utility>

namespace peek {
namespace util{

  template <typename Pair>
  Pair makeEmpty() {
    return std::pair<typename Pair::first_type, typename Pair::second_type>();
  }

  template <typename Pair>
  bool isEmpty(Pair p) {
    return !p.second && !p.first;
  }

}
}

#endif // PEEKLIB_UTIL_BUILDER_H