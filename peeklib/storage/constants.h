#ifndef PEEKLIB_STORAGE_CONSTANTS_H
#define PEEKLIB_STORAGE_CONSTANTS_H

namespace peek {
namespace storage{

  const int kBlockSize = 512;
  const int kSoftMaxFileSize = 20 * kBlockSize;

  const char* const kRecordFilePrefix = "/peekdata/data-";

}
}

#endif // PEEKLIB_STORAGE_CONSTANTS_H