#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>

#include "peeklib/storage/constants.h"
#include "peeklib/storage/persistent.h"
#include "peeklib/util/crc.h"

class Record : public Persistent {
  public:
    Record(std::string key_p, std::string value_p);
    Record(std::fstream& input, std::streampos pos);
    Record(std::shared_ptr<std::string> key_p, std::shared_ptr<std::string> value_p);
    Record();

    int write(std::fstream& output, std::streampos absolute) override;
    int load(std::fstream& input, std::streampos absolute) override;
    bool equal(const Record& other) const;
    int FileSize() const;

  private:
    // data members
    std::shared_ptr<std::string> key;
    std::shared_ptr<std::string> value;

    // internal struct for header and associated functions
    struct Header {
      int valid;
      int keySize;
      int valSize;
      int padSize;
      char padding[16 - sizeof(Checksum_t)];
      Checksum_t checksum;
    } __attribute__((packed));
    const int kHeaderSize = sizeof(Header);

    std::unique_ptr<Header> createHeader();

    // private member helper functions
    int calculatePaddingSize() const;

    template <typename BackInserterT>
    static void read(std::fstream& input, int n, BackInserterT insert);

    void validateRead(const Checksum_t keyChecksum_fromfile,
                      const Checksum_t valChecksum_fromfile);
};

#endif // RECORD_H