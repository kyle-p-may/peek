#ifndef PEEKLIB_UTIL_WRITEHELPER_H
#define PEEKLIB_UTIL_WRITEHELPER_H

#include <memory>

namespace peek{
namespace util{

    template <typename T>
    T* index(const char* const ptr, int offset)
    {
        return (T*) (ptr + offset);
    }

}
}



#endif // PEEKLIB_UTIL_WRITEHELPER_H