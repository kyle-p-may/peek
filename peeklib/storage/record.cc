#include "peeklib/storage/record.h"

#include <cassert>

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

Record::Record(std::ifstream& input, std::streampos pos)
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
    int headerSize = 32;
    int keyAndCheckSize = key->size() + sizeof(Checksum_t);
    int valueAndCheckSize = value->size() + sizeof(Checksum_t);
    int currentSize = headerSize + keyAndCheckSize + valueAndCheckSize;

    return (peek::storage::kBlockSize - (currentSize % peek::storage::kBlockSize)); 
}

template <typename BackInserterT>
void Record::read(std::ifstream& input, int n, BackInserterT insert)
{
    char c;

    for (int i = 0; i < n; ++i) {
        input.get(c);
        insert = c;
    }
}

/**
 * == START HEADER
 * Valid (4 byte)
 * Key Size (4 bytes)
 * Value size (4 bytes)
 * Padding size (4 bytes)
 * Expansion Space (12 bytes)
 * Header checksum (4 bytes)
 * == END HEADER
 * Key
 * Key Checksum (4 bytes)
 * ==
 * Value
 * Value Checksum (4 bytes)
 * ==
 * Padding
 * 
 * ============================
 * This is the format of a single record on disk
 * The header is 32 bytes in total (with empty slots for expansion)
 * and then each section has its own checksum
 * 
 * The total size of the record is:
 * 32 + 8 + padding size + key size + value size
 * 
 * Padding size is added to ensure that it is to the nearest
 * peek::storage::kBlockSize multiple
 * 
 * A side effect of this function is that it can modify the
 * read/write position of the given stream
 */
void
Record::write(std::ofstream& output, std::streampos absolute)
{
    // first, create header
    char header[kHeaderSize];

    const int valid = 1;
    const int keySize = int(key->size());
    const int valSize = int(value->size());
    const int paddingSize = calculatePaddingSize();
    const Checksum_t keyChecksum = CRC32::Calculate(key->begin(), key->end());
    const Checksum_t valChecksum = CRC32::Calculate(value->begin(), value->end());
    Checksum_t headerChecksum;

    auto totalSize = kHeaderSize + keySize + sizeof(Checksum_t) +
        valSize + sizeof(Checksum_t) + paddingSize;
    
    assert((totalSize % peek::storage::kBlockSize) == 0);

    *peek::util::index<int>(header, kValidPos) = valid;
    *peek::util::index<int>(header, kKeySizePos) = keySize;
    *peek::util::index<int>(header, kValSizePos) = valSize;
    *peek::util::index<int>(header, kPadSizePos) = paddingSize;
    headerChecksum = CRC32::Calculate(header, header + kCheckPos);
    *peek::util::index<Checksum_t>(header, kCheckPos) = headerChecksum;

    // now, write everything to disk
    try {
        output.seekp(absolute);
        output.write(header, kHeaderSize);

        output.write(key->c_str(), keySize);
        output.write((char*)(&keyChecksum), sizeof(keyChecksum));

        output.write(value->c_str(), valSize);
        output.write((char*)(&valChecksum), sizeof(valChecksum));

    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedWrite("Error: failed record write");
    }
}

int
Record::load(std::ifstream& input, std::streampos absolute)
{
    // TODO: handle the case that input is not opened correctly
    int valid;
    int keySize;
    int paddingSize;
    int valSize;
    Checksum_t headerChecksum_fromfile, valChecksum_fromfile, keyChecksum_fromfile;

    char header[kHeaderSize];

    // first read the header
    try {
        input.seekg(absolute);
        input.read((char*)(header), kHeaderSize);

        valid = *peek::util::index<int>(header, kValidPos);
        keySize = *peek::util::index<int>(header,kKeySizePos);
        valSize = *peek::util::index<int>(header, kValSizePos);
        paddingSize = *peek::util::index<int>(header, kPadSizePos);
        headerChecksum_fromfile = *peek::util::index<Checksum_t>(header, kCheckPos);
    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedRead("Error: failed reading header");
    }

    // process header
    if (valid != 1) {
        throw peek::storage::InvalidRecord("Error: read invalid record");
    }

    auto headerChecksum = CRC32::Calculate(header, header + kCheckPos);

    auto log_header = [&]() {
        std::ostringstream out;
        out << "header checksum" << std::endl <<
            "from file: " << headerChecksum_fromfile << std::endl <<
            "just now: " << headerChecksum << std::endl;
        return out.str();
    }; Debug::log(log_header);

    if (headerChecksum  != headerChecksum_fromfile) {
        throw peek::storage::CorruptedData("Error: header corrupted");
    }

    // if successful, make buffers for key and value
    key = std::make_shared<std::string>();
    value = std::make_shared<std::string>();
    key->reserve(keySize);
    value->reserve(valSize);

    // read the rest of the record
    try {
        read(input, keySize, std::back_inserter(*key));
        input.read((char*)(&keyChecksum_fromfile), sizeof(keyChecksum_fromfile));
        read(input, valSize, std::back_inserter(*value));
        input.read((char*)(&valChecksum_fromfile), sizeof(valChecksum_fromfile));

    } catch (std::ios_base::failure& e) {
        throw peek::storage::FailedRead("Error: failed reading key and val");
    }

    // process the key and value checksums
    auto keyChecksum = CRC32::Calculate(key->begin(), key->end());
    auto valChecksum = CRC32::Calculate(value->begin(), value->end());

    if (keyChecksum != keyChecksum_fromfile) {
        throw peek::storage::CorruptedData("Error: key corrupted");
    }

    if (valChecksum != valChecksum_fromfile) {
        throw peek::storage::CorruptedData("Error: value corrupted");
    }

    // the total size of this record on disk, returned
    // the header, the key, the value, the padding, and the two checksums for val and key
    return kHeaderSize + keySize + valSize + paddingSize + (2 * sizeof(Checksum_t));
}