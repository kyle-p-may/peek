#include "gtest/gtest.h"
#include "peek_client.h"

TEST(PeekClientTest, WillConstruct) {
    PeekClient client("test:1234");
}