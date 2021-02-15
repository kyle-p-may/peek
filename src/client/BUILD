load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "client",

    srcs = [
        "//src/common/protos:peek_cc_grpc",
    ] + glob(["lib/*.h"]),

    hdrs = [
        "lib/grpc_client.h",
        "lib/generic_client.h",
        "lib/monolithic_client.h",
        "lib/client_exceptions.h",
    ],

    deps = [
        "//src/common/protos:peek_cc_grpc",
        "//src/common/protos:peek_cc_proto",
        "//src/server:server",
        "@com_github_grpc_grpc//:grpc++",
    ],

    strip_include_prefix = "lib",

    visibility = [
        "//visibility:public",
    ],
)