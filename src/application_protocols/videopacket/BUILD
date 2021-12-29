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
        ":codec_lib",
        ":pkg_cc_proto",
        "@envoy//include/envoy/registry",
        "@meta_protocol_proxy//src/meta_protocol_proxy/codec:factory_lib",
    ],
)

envoy_cc_library(
    name = "codec_lib",
    repository = "@envoy",
    srcs = ["videopacket_codec.cc"],
    hdrs = ["videopacket_codec.h"],
    deps = [
        "@envoy//include/envoy/buffer:buffer_interface",
        "@envoy//source/common/common:logger_lib",
        "@envoy//source/common/buffer:buffer_lib",
        "@meta_protocol_proxy//src/meta_protocol_proxy/codec:codec_interface",
        "//src/application_protocols/videopacket/jce:jce",
        ":static",
    ],
)

envoy_cc_library(
    name = "static",
    repository = "@envoy",
    srcs = ["video_packet.cc"],
    hdrs = ["video_packet.h", "VideoCommHead.h", "FeedListRequest.h"],
    deps = [
        "//src/application_protocols/videopacket/jce:jce",
    ],
)

