load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "peek_proto",
    srcs = ["peek.proto"],
)

cc_proto_library(
    name = "peek_cc_proto",
    deps = [":peek_proto"],

    visibility = [
        "//visibility:public",
    ],
)

cc_grpc_library(
    name = "peek_cc_grpc",
    srcs = [":peek_proto"],
    grpc_only = True,
    deps = [
        ":peek_cc_proto"
    ],
    visibility = [
        "//visibility:public",
    ],
    use_external = True,
)