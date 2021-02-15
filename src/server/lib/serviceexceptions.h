#ifndef SERVICEEXCEPTIONS_H
#define SERVICEEXCEPTIONS_H

#include <exception>
#include <string>

namespace server {

    class UnknownKeyException : std::runtime_error {
      public:
        UnknownKeyException(const std::string& bad_key)
        : std::runtime_error(std::string("Key not found: ") + bad_key)
        {}
    };

}

#endif // SERVICEEXCEPTIONS_H