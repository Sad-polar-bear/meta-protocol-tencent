load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)

# compile proto
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_package")

api_proto_package(
    deps = ["@com_github_cncf_udpa//udpa/annotations:pkg"],
)

envoy_cc_library(
    name = "config",
    visibility = ["//:__pkg__"],
    repository = "@envoy",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    deps = [
        ":pkg_cc_proto",
        ":codec_lib",
        "@envoy//include/envoy/registry",
        "@meta_protocol_proxy//src/meta_protocol_proxy/codec:factory_lib",
    ],
)

envoy_cc_library(
    name = "codec_lib",
    repository = "@envoy",
    srcs = ["trpc_codec.cc"],
    hdrs = ["trpc_codec.h"],
    deps = [
        "@envoy//include/envoy/buffer:buffer_interface",
        "@envoy//source/common/common:logger_lib",
        "@envoy//source/common/buffer:buffer_lib",
        "@meta_protocol_proxy//src/meta_protocol_proxy/codec:codec_interface",
        ":codec_checker",
        ":protocol",
    ],
)

envoy_cc_library(
    name = "codec_checker",
    srcs = [
        "codec_checker.cc",
    ],
    hdrs = [
        "codec_checker.h",
    ],
    repository = "@envoy",
    deps = [
        ":pkg_cc_proto",
        ":protocol",
        "@envoy//include/envoy/buffer:buffer_interface",
        "@envoy//include/envoy/server:filter_config_interface",
        "@envoy//source/common/buffer:buffer_lib",
        "@envoy//source/common/common:assert_lib",
        "@envoy//source/common/common:minimal_logger_lib",
    ],
)

envoy_cc_library(
    name = "metadata",
    srcs = ["metadata.cc"],
    hdrs = ["metadata.h"],
    repository = "@envoy",
    deps = [
        ":pkg_cc_proto",
        "@envoy//source/common/http:header_map_lib",
    ],
)

envoy_cc_library(
    name = "protocol",
    srcs = ["protocol.cc"],
    hdrs = ["protocol.h"],
    repository = "@envoy",
    deps = [
        ":metadata",
        ":pkg_cc_proto",
        "@envoy//source/common/buffer:buffer_lib",
        "@envoy//source/common/common:minimal_logger_lib",
    ],
)

