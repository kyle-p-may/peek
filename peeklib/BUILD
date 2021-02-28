load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "peek_storage", 

    srcs = glob(["storage/*"]) + glob(["util/*"]),

    hdrs = [
        "storage/record.h",
        "util/crc.h",
        "storage/logentry.h",
    ],

    visibility = [
        "//tests:__subpackages__",
    ],
)