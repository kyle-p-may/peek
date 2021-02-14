#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H

#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "src/common/protos/peek.grpc.pb.h"
using peek::Peek;

class GrpcClient {
  public:
    /**
     * @param address IP address and port, in the following format
     *                  IP:port
     */
    GrpcClient(const std::string& address) : stub(nullptr)
    {
        std::cout << address << std::endl;
        throw std::logic_error("PeekClient ctor not implemented");
    }

    /**
     * @brief   This function will return the value for the key
     *          if it exists; if not, it will throw an exception
     *          (NonExistentKeyException)
     */
    std::string Get(const std::string& key)
    {
        throw std::logic_error("PeekClient::Get not implemented");
    }

    /**
     * @brief   Sets the value associated with key to value (overwriting 
     *          previous values if the key already existed)
     */
    void Set(const std::string& key, const std::string& value)
    {
        throw std::logic_error("PeekClient::Set not implemented");
    }
  
  private:
    std::unique_ptr<Peek::Stub> stub;
};

#endif // GRPC_CLIENT_H