#ifndef PEEKLIB_STORAGE_FILE_H
#define PEEKLIB_STORAGE_FILE_H

#include <string>

#include "peeklib/storage/constants.h"

using FileId = int;

namespace peek {
namespace storage{

  class FilenameGenerator {
    public:
      FilenameGenerator(const char* prefix_p)
      : prefix(prefix_p)
      { }

      std::string operator()(FileId fid) {
        return prefix + std::to_string(fid);
      }

    private:
      std::string prefix;
  };

}
}

#endif // PEEKLIB_STORAGE_FILE_H