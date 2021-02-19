cc_test(
    name = "record_tests",

    srcs = glob(["record/*.cc"]),

    copts = ["-Iexternal/gtest/include"],

    deps = [
        "@gtest//:gtest_main",
        "//peeklib:peek_storage",
    ],
)