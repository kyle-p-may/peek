#include "peeklib/storage/indexmap.h"

#include <memory>
#include <mutex>

#include "peeklib/storage/index.h"
#include "peeklib/util/access.h"

using namespace peek::util;

Guard<Unique<Index>> IndexMap::requestWrite(std::shared_ptr<Key> key) {
  IndexPtr p;
  {
    std::lock_guard<std::mutex> guard(m);

    if (map.count(*key) == 0) {
      return Guard<Unique<Index>>(nullptr);
    } else {
      p = map[*key];
    }
  }

  return Guard<Unique<Index>>(p);
}

Guard<Shared<Index>> IndexMap::requestRead(std::shared_ptr<Key> key) {
  IndexPtr p;
  {
    std::lock_guard<std::mutex> guard(m);

    if (map.count(*key) == 0) {
      return Guard<Shared<Index>>(nullptr);
    } else {
      p = map[*key];
    }
  }

  return Guard<Shared<Index>>(p);
}