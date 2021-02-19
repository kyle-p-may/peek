#ifndef PEEKLIB_STORAGE_PERSISTENT_H
#define PEEKLIB_STORAGE_PERSISTENT_H

#include <iostream>
#include <fstream>

class Persistent {
    public:
        virtual void write(std::ofstream& output, std::streampos absolute) = 0;
        virtual void load(std::ifstream& input, std::streampos absolute) = 0;
        virtual ~Persistent() = default;
};

#endif // PEEKLIB_STORAGE_PERSISTENT_H