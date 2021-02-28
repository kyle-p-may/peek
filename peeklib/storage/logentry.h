#ifndef PEEKLIB_STORAGE_LOGENTRY_H
#define PEEKLIB_STORAGE_LOGENTRY_H

#include <ios>
#include <iostream>
#include <memory>

#include "peeklib/storage/file.h"
#include "peeklib/storage/persistent.h"
#include "peeklib/util/crc.h"

class LogEntry : public Persistent {
    public:
        enum Type {
            Null,
            Delete,
            Write
        };

        LogEntry() = default;
        LogEntry(Type type_p, FileId target_file_p, std::streampos target_pos_p);
        LogEntry(std::fstream& input, std::streampos absolute);

        void finish(std::fstream& output);
        bool hasFinished() const;
        void commit(std::fstream& output);
        bool hasCommitted() const;
        void markBegin();
        bool isBegin() const;
        void markEnd();
        bool isEnd() const;

        void write(std::fstream& output);

        int write(std::fstream& output, std::streampos absolute) override;
        int load(std::fstream& input, std::streampos absolute) override;

    private:
        // data members
        Type type;
        FileId target_file;
        std::streampos target_pos;
        std::streampos location;
        bool begin;
        bool end;
        bool finished;
        bool committed;

        // header for writing to disk
        static const int kDesiredSize = 32;
        static const int kPaddingSize = kDesiredSize - sizeof(int) - \
            sizeof(FileId) - sizeof(int) - (4 * sizeof(bool)) - sizeof(Checksum_t);

        struct Header {
            int pos;
            FileId file;
            int type;
            bool begin;
            bool end;
            bool finished;
            bool committed;
            char padding[kPaddingSize];
            Checksum_t checksum;
        } __attribute__((packed));

        static const int kHeaderSize = sizeof(Header);

        // private helper functions
        std::unique_ptr<Header> createHeader();
        void initFromHeader(std::unique_ptr<Header>& header, std::streampos pos);
};

#endif // PEEKLIB_STORAGE_LOGENTRY_H