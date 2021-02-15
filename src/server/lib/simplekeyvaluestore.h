#ifndef SIMPLEKEYVALUESTORE_H
#define SIMPLEKEYVALUESTORE_H

#include <sstream>
#include <unordered_map>

#include "key.h"
#include "keyvalueservice.h"
#include "serviceexceptions.h"
#include "value.h"

class SimpleKeyValueStore : public KeyValueService
{
  public:

    SimpleKeyValueStore(std::istream& seed_data)
    : KeyValueService(), store()
    {
        initialize(seed_data);
    }

    SimpleKeyValueStore()
    : KeyValueService(), store()
    { }

    Value Get(const Key& key) override
    {
        if (store.count(key) <= 0) {
            throw server::UnknownKeyException(key);
        }

        return store[key];
    }

    void Set(const Key& key, const Value& value) override
    {
        store[key] = value;
    }

    virtual ~SimpleKeyValueStore() = default;

  private:
    std::unordered_map<Key, Value> store;

    virtual void initialize(std::istream& input) override
    {
        Key k;
        Value v;

        while (input >> k >> v) {
            store[k] = v;
        }
    }    
};

#endif // SIMPLEKEYVALUESTORE_H