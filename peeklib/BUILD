load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "peek_storage", 

    srcs = glob(["storage/*"]) + glob(["util/*"]),

    hdrs = [
        "storage/file.h",
        "storage/filemanager.h",
        "storage/logentry.h",
        "storage/record.h",
        "util/access.h",
        "util/crc.h",
    ],

    visibility = [
        "//tests:__subpackages__",
    ],
)