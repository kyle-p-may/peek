#ifndef PEEKLIB_STORAGE_INDEX_H
#define PEEKLIB_STORAGE_INDEX_H

#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "peeklib/storage/file.h"
#include "peeklib/storage/value.h"

using namespace peek;

class Index {
  public:
    Index(FileId fid_p, std::streampos location_p, std::shared_ptr<Value> data_p);

    void flushData();
    void addData(std::shared_ptr<Value> data_p);
    bool empty() const;

    std::shared_lock<std::shared_mutex> acquireReadLock();
    std::unique_lock<std::shared_mutex> acquireWriteLock();

  private:
    FileId fid;
    std::streampos location;
    std::shared_ptr<std::string> data;
    std::shared_mutex m;
};

using IndexPtr = std::shared_ptr<Index>;

#endif // PEEKLIB_STORAGE_INDEX_H