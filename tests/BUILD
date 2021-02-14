cc_test(
    name = "client_tests",
    srcs = glob(["client/*.cc"]),
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:gtest_main",
        "//src/client:grpc_client",
    ],
)