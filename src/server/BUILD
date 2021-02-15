load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "server",
    srcs = [
        "//src/common/protos:peek_cc_grpc",
    ] + glob(["lib/*.h"]),

    hdrs = glob(["lib/*.h"], allow_empty = False),

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