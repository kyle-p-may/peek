#include "peeklib/storage/index.h"

#include <cassert>
#include <iostream>
#include <string>
#include <memory>
#include <shared_mutex>

#include "peeklib/storage/file.h"

Index::Index(FileId fid_p, std::streampos location_p, std::shared_ptr<std::string> data_p)
: fid(fid_p), location(location_p), data(data_p), m()
{ }

void Index::flushData() {
  assert(!empty());
  data = nullptr;
}

void Index::addData(std::shared_ptr<std::string> data_p) {
  assert(empty());
  data = data_p;
}

bool Index::empty() const {
  return data == nullptr;
}

std::shared_lock<std::shared_mutex> Index::acquireReadLock() {
  return std::shared_lock<std::shared_mutex>(m);
}

std::unique_lock<std::shared_mutex> Index::acquireWriteLock() {
  return std::unique_lock<std::shared_mutex>(m);
}