#ifndef PEEKLIB_STORAGE_INDEXMAP_H
#define PEEKLIB_STORAGE_INDEXMAP_H

#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

#include "peeklib/storage/index.h"
#include "peeklib/storage/key.h"

using namespace peek;

using WriteAccess = std::pair<std::unique_lock<std::shared_mutex>, IndexPtr>;
using ReadAccess = std::pair<std::shared_lock<std::shared_mutex>, IndexPtr>;

class IndexMap {
  public:
    WriteAccess requestWrite(std::shared_ptr<Key> key);
    ReadAccess requestRead(std::shared_ptr<Key> key);
  private:
    std::unordered_map<Key, IndexPtr> map;
    std::mutex m;
};

#endif //PEEKLIB_STORAGE_INDEXMAP_H