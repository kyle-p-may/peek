#ifndef KEYVALUESERVICE_H
#define KEYVALUESERVICE_H

#include <iostream>

#include "key.h"
#include "value.h"

class KeyValueService {
  public:
    KeyValueService(std::istream& seed_data) {
        initialize(seed_data);
    }

    virtual Value Get(const Key&) = 0;
    virtual void Set(const Key&, const Value&) = 0;

  private:
    virtual void initialize(std::istream&) = 0; 
};

#endif // KEYVALUESERVICE_H