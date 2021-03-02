#ifndef PEEKLIB_STORAGE_INDEXMAP_H
#define PEEKLIB_STORAGE_INDEXMAP_H

#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

#include "peeklib/storage/index.h"
#include "peeklib/storage/key.h"
#include "peeklib/util/access.h"

using namespace peek;
using namespace peek::util;

class IndexMap {
  public:
    Guard<Unique<Index>> requestWrite(std::shared_ptr<Key> key);
    Guard<Shared<Index>> requestRead(std::shared_ptr<Key> key);
  private:
    std::unordered_map<Key, IndexPtr> map;
    std::mutex m;
};

#endif //PEEKLIB_STORAGE_INDEXMAP_H