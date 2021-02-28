#ifndef PEEKLIB_UTIL_WRITEHELPER_H
#define PEEKLIB_UTIL_WRITEHELPER_H

#include <memory>

namespace peek{
namespace util{

    template <typename T>
    const T* index(const char* const ptr, int offset)
    {
        const char* const pos = ptr + offset;
        return reinterpret_cast<const T*>(pos);
    }

    template <typename T>
    T* index(char* const ptr, int offset)
    {
        char* const pos = ptr + offset;
        return reinterpret_cast<T*>(pos);
    }

}
}



#endif // PEEKLIB_UTIL_WRITEHELPER_H