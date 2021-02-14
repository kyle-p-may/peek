#include "gtest/gtest.h"
#include "grpc_client.h"

TEST(PeekClientTest, WillConstruct) {
    GrpcClient client("test:1234");
}