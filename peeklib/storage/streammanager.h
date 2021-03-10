#ifndef PEEKLIB_STORAGE_STREAMMANAGER_H
#define PEEKLIB_STORAGE_STREAMMANAGER_H

#include <cassert>
#include <ios>
#include <iostream>
#include <fstream>
#include <mutex>

#include "peeklib/storage/file.h"
#include "peeklib/util/access.h"

class StreamManager : public peek::util::LockableResource {
  public:
    StreamManager(const std::string& filename, FileId fid_p)
    : peek::util::LockableResource()
    , stream(filename, std::ios::binary | std::ios::out | std::ios::in)
    , fid(fid_p)
    {
      assert(stream.is_open());
    }

    // StreamOperation takes a single fstream reference
    // as an input
    template <typename RetVal, typename StreamOperation>
    RetVal perform(StreamOperation op) {
      return op(stream);
    }

    FileId id() const {
      return fid;
    }

    ~StreamManager() {
      stream.close();
    }

    std::streampos Size() {
      assert(stream.is_open());

      stream.seekg(0, std::ios::end);

      return stream.tellg();
    }

    StreamManager(const StreamManager&) = delete;
    StreamManager& operator=(const StreamManager&) = delete;

    static void touch(const std::string& filename) {
      // this creates the file on the file system
      // we need to call touch on new file names
      std::ofstream s(filename);
      assert(s.is_open());
      s.close();
    }

  private:
    std::fstream stream;
    FileId fid;
};

#endif // PEEKLIB_STORAGE_STREAMMANAGER_H