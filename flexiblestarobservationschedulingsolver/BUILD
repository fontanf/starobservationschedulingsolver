load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
config_setting(name = "clp_build", values = {"define": "clp=true"}, visibility = ["//visibility:public"])
config_setting(name = "cplex_build",  values = {"define": "cplex=true"},  visibility = ["//visibility:public"])
config_setting(name = "xpress_build", values = {"define": "xpress=true"}, visibility = ["//visibility:public"])

cc_library(
        name = "flexiblestarobservationschedulingsolver",
        hdrs = [
                "instance.hpp",
                "solution.hpp",
                "utils.hpp",
        ],
        srcs = [
                "instance.cpp",
                "solution.cpp",
                "utils.cpp",
        ],
        deps = [
                "//starobservationschedulingsolver:starobservationschedulingsolver",
                "@optimizationtools//optimizationtools/utils:info",
                "@optimizationtools//optimizationtools/utils:utils",
                "@optimizationtools//optimizationtools/containers:containers",
        ],
        visibility = ["//visibility:public"],
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [
                "//flexiblestarobservationschedulingsolver/algorithms:algorithms",
        ],
        linkopts = select({
                    "@bazel_tools//src/conditions:windows": [],
                    "//conditions:default":                 ["-lpthread"],
        }),
)

cc_binary(
        name = "sosp2fsosp",
        srcs = ["sosp2fsosp.cpp"],
        deps = [
                "//flexiblestarobservationschedulingsolver:flexiblestarobservationschedulingsolver",
                "@boost//:program_options",
        ],
)

