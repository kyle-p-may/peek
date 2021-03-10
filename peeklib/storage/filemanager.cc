#include "peeklib/storage/filemanager.h"

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

#include "peeklib/storage/constants.h"
#include "peeklib/storage/file.h"
#include "peeklib/storage/streammanager.h"

FileManager::FileManager(const std::vector<FileId>& valid_fids_p, peek::storage::FilenameGenerator g_p)
: current_file_id(), map(), m(), g(g_p)
{
  if (!valid_fids_p.empty()) {
    init(valid_fids_p);
  }

  // should be tracking the same number of files
  assert(valid_fids_p.size() == map.size());
}

Guard<Unique<StreamManager>> FileManager::getPermission(FileId fid) {
  std::shared_ptr<StreamManager> p(nullptr);
  {
    std::lock_guard<std::mutex> guard(m);
    p = access(fid);
  }
  return Guard<Unique<StreamManager>>(p);
}

Guard<Unique<StreamManager>> FileManager::getPermission() {
  std::shared_ptr<StreamManager> p(nullptr);
  {
    std::lock_guard<std::mutex> guard(m);

    {
      Guard<Unique<StreamManager>> stream_guard(access(current_file_id));

      // check to see if this stream can be added to or if we should make a new file
      std::streampos filesize = stream_guard.Resource().Size();

      if (filesize <= peek::storage::kSoftMaxFileSize) {
        return stream_guard;
      }
    }

    ++current_file_id;
    map[current_file_id] = nullptr;
    StreamManager::touch(g(current_file_id));
    addFile(current_file_id);
    p = access(current_file_id);
  }
  return Guard<Unique<StreamManager>>(p);
}

void FileManager::init(const std::vector<FileId>& valid_fids_p) {
  assert(!valid_fids_p.empty());

  auto max_el = std::max_element(valid_fids_p.begin(), valid_fids_p.end());
  current_file_id = *max_el;

  for (auto fid : valid_fids_p) {
    map[fid] = nullptr;
  }
}

void FileManager::addFile(FileId fid) {
  auto filename = g(fid);
  assert(map[fid] == nullptr);
  map[fid] = std::make_shared<StreamManager>(filename, fid);
}

std::shared_ptr<StreamManager>& FileManager::access(FileId fid) {
  assert(map.count(fid) > 0);

  if (!map[fid]) {
    addFile(fid);
    assert(map[fid]);
  }

  return map[fid];
}