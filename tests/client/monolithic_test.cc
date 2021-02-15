#include <memory>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "monolithic_client.h"
#include "simplekeyvaluestore.h"

TEST(SimpleTest, CanConstruct) {
    std::istringstream seed("key/1 blahblah\nkey/2 nahaha\n");
    auto kvs1 = std::make_unique<SimpleKeyValueStore>();
    auto kvs2 = std::make_unique<SimpleKeyValueStore>(seed);
    MonolithicClient<SimpleKeyValueStore> client(kvs1);
    MonolithicClient<SimpleKeyValueStore> seeded_client(kvs2);
}

TEST(SimpleTest, CanGet) {
    std::istringstream seed("key/1 blahblah\nkey/2 nahaha\n");
    auto kvs = std::make_unique<SimpleKeyValueStore>(seed);
    MonolithicClient<SimpleKeyValueStore> seeded_client(kvs);
    std::string value1 = "blahblah";
    std::string value2 = "nahaha";

    ASSERT_EQ(seeded_client.Get("key/1"), value1);
    ASSERT_EQ(seeded_client.Get("key/2"), value2);
}