#ifndef PEEKLIB_DRIVER_WORKER_H
#define PEEKLIB_DRIVER_WORKER_H

#include <cassert>
#include <iostream>
#include <memory>

#include "peeklib/storage/exceptions.h"
#include "peeklib/storage/indexmap.h"
#include "peeklib/storage/key.h"
#include "peeklib/storage/value.h"
#include "peeklib/util/access.h"
#include "peeklib/util/debug.h"

void performWrite(std::shared_ptr<Key> key, std::shared_ptr<Value> val, std::shared_ptr<IndexMap> map) {
  try {
    auto writeAccess = map->requestWrite(key);

    // now, build our log

  } catch (peek::storage::CorruptedData& e) {
    auto errorPrint = [&] (std::ostream& out) {
      out << "performWrite driver failed" << std::endl;
      out << "Error type: " << e.what() << std::endl;
      out << "Dropping back to recovery mode" << std::endl;
    };

    Debug::log(errorPrint);
    assert(false);
    // TODO this is unimplemented
  }
}

std::shared_ptr<Value> performRead(std::shared_ptr<Key> key) {

}

#endif // PEEKLIB_DRIVER_WORKER_H