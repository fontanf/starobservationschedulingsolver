load("@bazel_skylib//lib:selects.bzl", "selects")

config_setting(
    name = "clp_build",
    values = {"define": "clp=true"},
    visibility = ["//visibility:public"],
)

selects.config_setting_group(
    name = "clp_linux",
    match_all = [":clp_build", "@bazel_tools//src/conditions:linux"],
)

selects.config_setting_group(
    name = "clp_windows",
    match_all = [":clp_build", "@bazel_tools//src/conditions:windows"],
)

config_setting(
    name = "cplex_build",
    values = {"define": "cplex=true"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "xpress_build",
    values = {"define": "xpress=true"},
    visibility = ["//visibility:public"]
)

load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
