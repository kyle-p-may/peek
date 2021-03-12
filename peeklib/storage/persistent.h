#ifndef PEEKLIB_STORAGE_PERSISTENT_H
#define PEEKLIB_STORAGE_PERSISTENT_H

#include <iostream>
#include <fstream>

#include "peeklib/storage/streammanager.h"

class Persistent {
  public:
    virtual int write(StreamManager& man, std::streampos absolute) {
      auto action = [&](std::fstream& stream) {
        return this->write(stream, absolute);
      };
      return man.perform(action);
    }

    virtual int load(StreamManager& man, std::streampos absolute) {
      auto action = [&](std::fstream& stream) {
        return this->load(stream, absolute);
      };
      return man.perform(action);
    }

    virtual int write(std::fstream& output, std::streampos absolute) = 0;
    virtual int load(std::fstream& input, std::streampos absolute) = 0;
    virtual ~Persistent() = default;
};

#endif // PEEKLIB_STORAGE_PERSISTENT_H