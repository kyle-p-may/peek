load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "peek_client",
    srcs = [
        "lib/peek_client.h",
        "lib/client_exceptions.h",
    ],
    hdrs = [
        "lib/peek_client.h"
    ],
    deps = [
        "//src/common/protos:peek_cc_grpc",
        "//src/common/protos:peek_cc_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
    strip_include_prefix = "lib",

    visibility = [
        "//visibility:public",
    ],
)