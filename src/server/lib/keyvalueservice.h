#ifndef KEYVALUESERVICE_H
#define KEYVALUESERVICE_H

#include <iostream>

#include "key.h"
#include "value.h"

class KeyValueService {
  public:
    KeyValueService() = default;

    virtual Value Get(const Key&) = 0;
    virtual void Set(const Key&, const Value&) = 0;

    virtual ~KeyValueService() = default;

  private:
    virtual void initialize(std::istream&) { return; } 
};

#endif // KEYVALUESERVICE_H