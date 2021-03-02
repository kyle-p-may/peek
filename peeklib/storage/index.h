#ifndef PEEKLIB_STORAGE_INDEX_H
#define PEEKLIB_STORAGE_INDEX_H

#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "peeklib/storage/file.h"
#include "peeklib/storage/value.h"
#include "peeklib/util/access.h"

using namespace peek;

class Index : public util::LockableResource {
  public:
    Index(FileId fid_p, std::streampos location_p, std::shared_ptr<Value> data_p);

    void flushData();
    void addData(std::shared_ptr<Value> data_p);
    bool empty() const;

    std::shared_ptr<Value> read();

  private:
    FileId fid;
    std::streampos location;
    std::shared_ptr<std::string> data;
};

using IndexPtr = std::shared_ptr<Index>;

#endif // PEEKLIB_STORAGE_INDEX_H