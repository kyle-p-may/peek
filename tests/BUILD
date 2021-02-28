cc_test(
    name = "record_tests",

    srcs = glob(["record/*.cc"]),

    copts = ["-Iexternal/gtest/include"],

    defines = [
        "PEEK_DEBUG",
    ],

    deps = [
        "@gtest//:gtest_main",
        "//peeklib:peek_storage",
    ],
)

cc_test(
    name = "log_tests",

    srcs = glob(["log/*.cc"]),

    copts = ["-Iexternal/gtest/include"],

    defines = [
        "PEEK_DEBUG",
    ],

    deps = [
        "@gtest//:gtest_main",
        "//peeklib:peek_storage",
    ],
)