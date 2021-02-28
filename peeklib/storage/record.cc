#include "peeklib/storage/record.h"

#include <cassert>

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

#include "peeklib/storage/exceptions.h"
#include "peeklib/util/crc.h"
#include "peeklib/util/debug.h"
#include "peeklib/util/writehelper.h"

Record::Record(std::shared_ptr<std::string> key_p, std::shared_ptr<std::string> value_p)
: key(key_p), value(value_p)
{ } 

Record::Record(std::string key_p, std::string value_p)
: Record(std::make_shared<std::string>(key_p), std::make_shared<std::string>(value_p))
{ }

Record::Record(std::fstream& input, std::streampos pos)
: key(nullptr), value(nullptr)
{
    load(input, pos);
}

Record::Record()
: key(nullptr), value(nullptr)
{ }

bool
Record::equal(const Record& other) const
{
    // compare the underlying std::strings
    return *(other.key) == *(this->key) && *(other.value) == *(this->value);
}

int
Record::calculatePaddingSize() const
{
    const int headerSize = kHeaderSize;
    const int keyAndCheckSize = key->size() + sizeof(Checksum_t);
    const int valueAndCheckSize = value->size() + sizeof(Checksum_t);
    const int currentSize = headerSize + keyAndCheckSize + valueAndCheckSize;

    return (peek::storage::kBlockSize - (currentSize % peek::storage::kBlockSize)); 
}

std::unique_ptr<Record::Header>
Record::createHeader()
{
    const int valid = 1;
    const int keySize = int(key->size());
    const int valSize = int(value->size());
    const int paddingSize = calculatePaddingSize();
    auto header = std::make_unique<Record::Header>();
    header->valid = valid;
    header->keySize = keySize;
    header->valSize = valSize;
    header->padSize = paddingSize;
    header->checksum = CRC32::Calculate(
        reinterpret_cast<char*>(header.get()), 
        reinterpret_cast<char*>(&(header->checksum))
    );

    return header;
}

template <typename BackInserterT>
void Record::read(std::fstream& input, int n, BackInserterT insert)
{
    char c;

    for (int i = 0; i < n; ++i) {
        input.get(c);
        insert = c;
    }
}

/**
 * A side effect of this function is that it can modify the
 * read/write position of the given stream
 */
int
Record::write(std::fstream& output, std::streampos absolute)
{
    if (!output.is_open()) {
        throw peek::storage::FailedWrite("Error: Record::write stream not open");
    }

    auto header = createHeader();
    const Checksum_t keyChecksum = CRC32::Calculate(key->begin(), key->end());
    const Checksum_t valChecksum = CRC32::Calculate(value->begin(), value->end());

    // now, write everything to disk
    try {
        output.seekp(absolute);
        output.write(reinterpret_cast<char*>(header.get()), kHeaderSize);

        output.write(key->c_str(), key->size());
        output.write((char*)(&keyChecksum), sizeof(keyChecksum));

        output.write(value->c_str(), value->size());
        output.write((char*)(&valChecksum), sizeof(valChecksum));

        output.flush();

    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedWrite("Error: failed record write");
    }

    return FileSize();
}

int
Record::load(std::fstream& input, std::streampos absolute)
{
    if (!input.is_open()) {
        throw peek::storage::FailedRead("Error: Record::load input not open");
    }

    auto header = std::make_unique<Header>();
    try {
        input.seekg(absolute);
        input.read(reinterpret_cast<char*>(header.get()), kHeaderSize);
    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedRead("Error: failed reading header");
    }

    // if successful, make buffers for key and value
    key = std::make_shared<std::string>();
    value = std::make_shared<std::string>();
    key->reserve(header->keySize);
    value->reserve(header->valSize);

    // read the rest of the record
    Checksum_t valChecksum_fromfile, keyChecksum_fromfile;
    try {
        read(input, header->keySize, std::back_inserter(*key));
        input.read((char*)(&keyChecksum_fromfile), sizeof(keyChecksum_fromfile));
        read(input, header->valSize, std::back_inserter(*value));
        input.read((char*)(&valChecksum_fromfile), sizeof(valChecksum_fromfile));

    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedRead("Error: failed reading key and val");
    }

    validateRead(keyChecksum_fromfile, valChecksum_fromfile);

    return FileSize();
}

void
Record::validateRead(const Checksum_t keyChecksum_fromfile, const Checksum_t valChecksum_fromfile)
{
    // process the key and value checksums
    auto keyChecksum = CRC32::Calculate(key->begin(), key->end());
    auto valChecksum = CRC32::Calculate(value->begin(), value->end());

    if (keyChecksum != keyChecksum_fromfile) {
        throw peek::storage::CorruptedData("Error: key corrupted");
    }

    if (valChecksum != valChecksum_fromfile) {
        throw peek::storage::CorruptedData("Error: value corrupted");
    }
}

int
Record::FileSize() const
{
    // the header, the key, the value, the padding, and the two checksums for val and key
    return kHeaderSize + key->size() + value->size() +
        calculatePaddingSize() + (2 * sizeof(Checksum_t));
}