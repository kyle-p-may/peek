#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "peeklib/storage/constants.h"
#include "peeklib/storage/persistent.h"
#include "peeklib/util/crc.h"

class Record : public Persistent {
    public:
        Record(std::string key_p, std::string value_p);
        Record(std::ifstream& input, std::streampos pos);
        Record(std::shared_ptr<std::string> key_p, std::shared_ptr<std::string> value_p);
        Record();

        void write(std::ofstream& output, std::streampos absolute) override;
        int load(std::ifstream& input, std::streampos absolute) override;

        bool equal(const Record& other) const;

    private:
        std::shared_ptr<std::string> key;
        std::shared_ptr<std::string> value;

        int calculatePaddingSize() const;

        template <typename BackInserterT>
        static void read(std::ifstream& input, int n, BackInserterT insert);

        const int kHeaderSize = 32;
        const int kValidPos = 0;
        const int kKeySizePos = kValidPos + sizeof(int);
        const int kValSizePos = kKeySizePos + sizeof(int);
        const int kPadSizePos = kValSizePos + sizeof(int);
        const int kCheckPos = kHeaderSize - sizeof(Checksum_t);
};

#endif // RECORD_H