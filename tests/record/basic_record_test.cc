#include <ios>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "peeklib/storage/constants.h"
#include "peeklib/storage/exceptions.h"
#include "peeklib/storage/record.h"

TEST(BasicRecordTest, WillConstructKv)
{
    Record r1("hello", "world");

    auto k = std::make_shared<std::string>("hello");
    auto v = std::make_shared<std::string>("world");
    auto nv = std::make_shared<std::string>("blah");

    Record r2(k, v);
    Record r3(k, nv);

    ASSERT_TRUE(r1.equal(r2));
    ASSERT_FALSE(r1.equal(r3));
    ASSERT_FALSE(r2.equal(r3));
    ASSERT_TRUE(r1.equal(r1));
    ASSERT_TRUE(r2.equal(r1));
}

TEST(BasicRecordTest, SaveAndRead)
{
    const char *filename = "/peekdata/SaveAndRead_data.bin";

    std::fstream output(filename, std::ios_base::binary | std::ios::out);

    EXPECT_TRUE(output.is_open());

    Record r1("key", "value");
    r1.write(output, 0);

    output.close();
    std::fstream input(filename, std::ios_base::binary | std::ios::in);

    EXPECT_TRUE(input.is_open());
    // now read from file, see if it works correctly
    Record r2;
    auto size = r2.load(input, 0);
    input.close();

    ASSERT_TRUE(r1.equal(r2));

    std::cout << "size: " << size << std::endl;

    // assumes that this small record will just be a single block
    EXPECT_TRUE(size == peek::storage::kBlockSize);
}

TEST(BasicRecordTest, HeaderCorrupt)
{
    const char *filename = "/peekdata/HeaderCorrupt_data.bin";
    std::vector<char> bad_header(32, 0);
    std::fstream output(filename, std::ios_base::binary | std::ios::out);
    EXPECT_TRUE(output.is_open());

    const char *key = "oaisdfoiajiejieofoeiaoiajesfioef";
    const char *value = "aoisdfoaienrtsiourggggoaisdfoiasedfijoef";
    auto key_p = std::make_shared<std::string>(key);
    auto val_p = std::make_shared<std::string>(value);

    // sync
    Record r1(key_p, val_p);
    r1.write(output, 0);

    // now, corrupt header
    output.seekp(4);
    output.write(bad_header.data(), 16);
    output.close();

    std::fstream input(filename, std::ios_base::binary | std::ios::in);
    EXPECT_TRUE(input.is_open());

    try
    {
        Record r2(input, 0);
        FAIL() << "Expected corrupted data exception";
    }
    catch (peek::storage::CorruptedData &e)
    {
        std::cout << "correct" << std::endl;
    }
    catch (...)
    {
        FAIL() << "wrong exception thrown";
    }
}

TEST(BasicRecordTest, TwoRecordsSameFile)
{
    const char *filename = "/peekdata/TwoRecordsSameFile_data.bin";
    std::fstream output(filename, std::ios_base::binary | std::ios::out);
    EXPECT_TRUE(output.is_open());

    const char *key1 = "key1";
    const char *value1 = "value1";
    const char *key2 = "key2";
    const char *value2 = "value2";

    auto key1_p = std::make_shared<std::string>(key1);
    auto value1_p = std::make_shared<std::string>(value1);

    auto key2_p = std::make_shared<std::string>(key2);
    auto value2_p = std::make_shared<std::string>(value2);

    std::streampos write_pos1 = 0;
    Record r1(key1_p, value1_p);
    Record r2(key2_p, value2_p);
    std::streampos write_pos2 = r1.write(output, write_pos1);
    (void)r2.write(output, write_pos2);
    output.close();

    // now, open and read both records
    std::fstream input(filename, std::ios_base::binary | std::ios::in);
    EXPECT_TRUE(input.is_open());
    
    // note the order
    Record r3(input, write_pos2); 
    Record r4(input, write_pos1);
    input.close();

    ASSERT_TRUE(r1.equal(r4));
    ASSERT_TRUE(r2.equal(r3));
}