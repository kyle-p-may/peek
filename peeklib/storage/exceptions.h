#ifndef PEEKLIB_STORAGE_EXCEPTIONS_H
#define PEEKLIB_STORAGE_EXCEPTIONS_H

#include <stdexcept>

namespace peek {
namespace storage {

class FailedWrite : public std::runtime_error {
    public:
        FailedWrite(const char* what_arg)
        : std::runtime_error(what_arg)
        { }
};

class CorruptedData : public std::runtime_error {
    public:
        CorruptedData(const char* what_arg)
        : std::runtime_error(what_arg)
        { }
};

class FailedRead : public std::runtime_error {
    public:
        FailedRead(const char* what_arg)
        : std::runtime_error(what_arg)
        { }
};

class InvalidRecord : public std::runtime_error {
    public:
        InvalidRecord(const char* what_arg)
        : std::runtime_error(what_arg)
        { }
};

}
}

#endif // PEEKLIB_STORAGE_EXCEPTIONS_H