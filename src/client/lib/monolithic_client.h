#ifndef MONOLITHIC_CLIENT_H
#define MONOLITHIC_CLIENT_H

#include <exception>
#include <memory>
#include <utility>

#include "client_exceptions.h"
#include "generic_client.h"
#include "keyvalueservice.h"
#include "serviceexceptions.h"

template <typename KeyValueServiceT>
class MonolithicClient : public Client {
  public:
    MonolithicClient(std::unique_ptr<KeyValueServiceT>& kvs)
    : kv(std::move(kvs))
    { }

    std::string Get(const std::string& key) override
    {
        try {
            return kv->Get(key);
        } catch (server::UnknownKeyException& e) {
            throw peek::NonExistentKeyException(key);
        }
    }

    void Set(const std::string& key, const std::string& value) override
    {
        kv->Set(key, value);
    }

  private:
    std::unique_ptr<KeyValueServiceT> kv;
};

#endif // MONOLITHIC_CLIENT_H