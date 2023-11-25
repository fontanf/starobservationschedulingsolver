cc_library(
        name = "starobservationschedulingsolver",
        hdrs = [
                "instance.hpp",
                "instance_builder.hpp",
                "solution.hpp",
        ],
        srcs = [
                "instance.cpp",
                "instance_builder.cpp",
                "solution.cpp",
        ],
        deps = [
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
                "//starobservationschedulingsolver/starobservationscheduling/algorithms:algorithms",
        ],
        linkopts = select({
                    "@bazel_tools//src/conditions:windows": [],
                    "//conditions:default": ["-lpthread"],
        }),
)

