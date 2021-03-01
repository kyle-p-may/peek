#include "peeklib/storage/indexmap.h"

#include <memory>
#include <mutex>

#include "peeklib/util/access.h"

WriteAccess IndexMap::requestWrite(std::shared_ptr<Key> key) {
  IndexPtr p;
  {
    std::lock_guard<std::mutex> guard(m);

    if (map.count(*key) == 0) {
      return peek::util::makeEmpty<WriteAccess>();
    } else {
      p = map[*key];
    }
  }

  return std::make_pair(p->acquireWriteLock(), p);
}

ReadAccess IndexMap::requestRead(std::shared_ptr<Key> key) {
  IndexPtr p;
  {
    std::lock_guard<std::mutex> guard(m);

    if (map.count(*key) == 0) {
      return peek::util::makeEmpty<ReadAccess>();
    } else {
      p = map[*key];
    }
  }

  return std::make_pair(p->acquireReadLock(), p);
}