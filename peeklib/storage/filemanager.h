#ifndef PEEKLIB_STORAGE_FILEMANAGER_H
#define PEEKLIB_STORAGE_FILEMANAGER_H

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "peeklib/storage/file.h"
#include "peeklib/storage/streammanager.h"
#include "peeklib/util/access.h"

using namespace peek::util;

class FileManager {
  public:
    FileManager(const std::vector<FileId>& valid_fids_p, peek::storage::FilenameGenerator g_p);

    // get read/write permission for certain file id
    // used when reading old key or updated old key
    Guard<Unique<StreamManager>> getPermission(FileId fid);

    // get read/write permission for the largest valid file id
    // used when adding new key
    Guard<Unique<StreamManager>> getPermission();

  private:
    FileId current_file_id;
    std::unordered_map<FileId, std::shared_ptr<StreamManager>> map;
    std::mutex m;
    peek::storage::FilenameGenerator g;

    void init(const std::vector<FileId>& valid_fids_p);
    void addFile(FileId file);
    std::shared_ptr<StreamManager>& access(FileId fid);
};

#endif // PEEKLIB_STORAGE_RECORDMANAGER_H