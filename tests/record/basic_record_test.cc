#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "peeklib/storage/record.h"

TEST(BasicRecordTest, WillConstructKv) {
    Record r1("hello", "world");

    auto k = std::make_shared<std::string>("hello");
    auto v = std::make_shared<std::string>("world");

    Record r2(k, v);

    EXPECT_TRUE(r1.equal(r2));
}