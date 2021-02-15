#ifndef CLIENT_EXCEPTIONS_H
#define CLIENT_EXCEPTIONS_H

#include <exception>
#include <string>

namespace peek {

class MalformedKeyException : std::runtime_error {
  public:
    MalformedKeyException(const std::string& bad_key)
    : std::runtime_error(std::string("Malformed Key: ") + bad_key)
    {}
};

class NonExistentKeyException : std::runtime_error {
  public:
    NonExistentKeyException(const std::string& bad_key)
    : std::runtime_error(std::string("Key not found: ") + bad_key)
    {}
};

class DuplicateKeyException : std::runtime_error {
  public:
    DuplicateKeyException(const std::string& bad_key)
    : std::runtime_error(std::string("Duplicate key: ") + bad_key)
    {}
};

}
#endif // CLIENT_EXCEPTIONS_H