#include "peeklib/storage/logentry.h"

#include "ios"
#include <iostream>
#include <memory>

#include "peeklib/storage/exceptions.h"

LogEntry::LogEntry(Type type_p, FileId target_file_p,
    std::streampos target_pos_p)
: type(type_p)
, target_file(target_file_p)
, target_pos(target_pos_p)
, location(std::streamoff(-1))
, begin(false)
, end(false)
, finished(false)
, committed(false)
{ }
  
LogEntry::LogEntry(std::fstream& input, std::streampos absolute)
: LogEntry()
{
    location = absolute;
    load(input, absolute);
}

void
LogEntry::markBegin()
{
    begin = true;
}

bool
LogEntry::isBegin() const
{
    return begin;
}

void
LogEntry::markEnd()
{
    end = true;
}

bool
LogEntry::isEnd() const
{
    return end;
}

void
LogEntry::commit(std::fstream& output)
{
    assert(finished);
    assert(!committed);

    committed = true;
    write(output);
}

void
LogEntry::finish(std::fstream& output)
{
    assert(!finished);
    finished = true;    

    write(output);
}

bool
LogEntry::hasFinished() const
{
    return finished;
}

bool
LogEntry::hasCommitted() const
{
    return committed;
}

std::unique_ptr<LogEntry::Header>
LogEntry::createHeader()
{
    auto header = std::make_unique<Header>();
    header->type = static_cast<int>(type);
    header->file = target_file;
    header->pos = target_pos;
    header->begin = begin;
    header->end = end;
    header->finished = finished;
    header->committed = committed;

    header->checksum = CRC32::Calculate(
        reinterpret_cast<char*>(header.get()),
        reinterpret_cast<char*>(&(header->checksum))
    );

    return header;
}

void 
LogEntry::initFromHeader(std::unique_ptr<LogEntry::Header>& header, std::streampos pos) {
    type = static_cast<Type>(header->type);
    target_file = header->file;
    target_pos = header->pos;
    location = pos;
    begin = header->begin;
    end = header->end;
    finished = header->finished;
    committed = header->committed;
}

void
LogEntry::write(std::fstream& output) {
    write(output, location);
}

int
LogEntry::write(std::fstream& output, std::streampos absolute)
{
    if (!output.is_open()) {
        throw peek::storage::FailedWrite("Error: LogEntry::write stream not open");
    }

    auto header = createHeader();

    try {
        output.seekp(absolute);
        output.write(reinterpret_cast<char*>(header.get()), kHeaderSize);
        output.flush();
    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedWrite("Error: failed logentry write");
    }

    return kHeaderSize;
}

int
LogEntry::load(std::fstream& input, std::streampos absolute)
{
    if (!input.is_open()) {
        throw peek::storage::FailedRead("Error: LogEntry::read stream not open");
    }

    auto header = std::make_unique<Header>();

    try {
        input.seekg(absolute);
        input.read(reinterpret_cast<char*>(header.get()), kHeaderSize);

    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedRead("Error: LogEntry::read failed");
    }

    // checksum before returning success
    auto checksum = CRC32::Calculate(
        reinterpret_cast<char*>(header.get()),
        reinterpret_cast<char*>(&(header->checksum))
    );

    if (checksum != header->checksum) {
        throw peek::storage::CorruptedData("Error: corrupted log entry");
    }
    initFromHeader(header, absolute);

    return kHeaderSize;
}