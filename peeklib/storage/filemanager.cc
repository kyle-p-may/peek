#include "peeklib/storage/filemanager.h"

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

#include "peeklib/storage/constants.h"
#include "peeklib/storage/file.h"
#include "peeklib/storage/streammanager.h"
#include "peeklib/util/debug.h"

FileManager::FileManager(const std::vector<FileId>& valid_fids_p, peek::storage::FilenameGenerator g_p)
: current_file_id(0), map(), m(), g(g_p)
{
  if (!valid_fids_p.empty()) {
    init(valid_fids_p);
    // should be tracking the same number of files
    assert(valid_fids_p.size() == map.size());
  }  else {
    map[current_file_id] = nullptr;
    StreamManager::touch(g(current_file_id));
  }
}

peek::util::GuardPtr<Unique<StreamManager>> FileManager::getPermission(FileId fid) {
  std::shared_ptr<StreamManager> p(nullptr);
  {
    std::lock_guard<std::mutex> guard(m);
    p = access(fid);
  }
  return std::make_unique<Guard<Unique<StreamManager>>>(p);
}

peek::util::GuardPtr<Unique<StreamManager>> FileManager::getPermission() {
  Debug::log([](auto& out) { out << "in getPermission" << std::endl; });
  std::shared_ptr<StreamManager> p(nullptr);
  {
    std::lock_guard<std::mutex> guard(m);
    Debug::log([](auto& out) { out << "acquired fm lock" << std::endl; });

    {
      Debug::log([&](auto& out) { out << "acquiring access to " << current_file_id << std::endl; });
      auto stream_guard = std::make_unique<Guard<Unique<StreamManager>>>(access(current_file_id));
      Debug::log([&](auto& out) { out << "acquired access to " << current_file_id << std::endl; });

      // check to see if this stream can be added to or if we should make a new file
      std::streampos filesize = stream_guard->UseResource([](auto s) {
        return s->Size();
      });

      if (filesize <= peek::storage::kSoftMaxFileSize) {
        Debug::log([&](auto& out) { out << "giving permission for " << current_file_id << std::endl; });
        return stream_guard;
      }
    }

    Debug::log([&](auto& out) { out << "giving permission for next id" << std::endl; });
    ++current_file_id;
    createFileForFirstTime(current_file_id);
    p = access(current_file_id);
  }
  return std::make_unique<Guard<Unique<StreamManager>>>(p);
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
  Debug::log([&](auto& out) { out << "adding file " << g(fid) << std::endl; });
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

void FileManager::createFileForFirstTime(FileId fid) {
  Debug::log([&](auto& out) { out << "creating " << g(fid) << " file for the first time" << std::endl; });
  map[fid] = nullptr;
  StreamManager::touch(g(fid));
  addFile(fid);
}