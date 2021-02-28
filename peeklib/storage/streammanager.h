#ifndef PEEKLIB_STORAGE_STREAMMANAGER_H
#define PEEKLIB_STORAGE_STREAMMANAGER_H

#include <ios>
#include <iostream>
#include <fstream>
#include <mutex>

#include "peeklib/storage/file.h"

class StreamManager {
    public:
        StreamManager(std::fstream& stream_p, FileId fid_p, std::mutex& m)
        : stream(stream_p), fid(fid_p), guard(m)
        { }

        // StreamOperation takes a single fstream reference
        // as an input
        template <typename RetVal, typename StreamOperation>
        RetVal perform(StreamOperation op) {
            return op(stream);
        }

        FileId Id() const {
            return fid;
        }

        ~StreamManager() {
            stream.close();
        }

    private:
        std::fstream& stream;
        FileId fid;
        std::lock_guard<std::mutex> guard;
};

#endif // PEEKLIB_STORAGE_STREAMMANAGER_H