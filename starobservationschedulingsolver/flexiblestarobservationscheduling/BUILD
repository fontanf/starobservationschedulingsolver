cc_library(
        name = "flexiblestarobservationschedulingsolver",
        hdrs = [
                "instance.hpp",
                "instance_builder.hpp",
                "solution.hpp",
                "utils.hpp",
                "algorithm_formatter.hpp",
        ],
        srcs = [
                "instance.cpp",
                "instance_builder.cpp",
                "solution.cpp",
                "utils.cpp",
                "algorithm_formatter.cpp",
        ],
        deps = [
                "//starobservationschedulingsolver/starobservationscheduling:starobservationschedulingsolver",
                "@optimizationtools//optimizationtools/utils:utils",
                "@optimizationtools//optimizationtools/containers:containers",
        ],
        visibility = ["//visibility:public"],
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [
                "//starobservationschedulingsolver/flexiblestarobservationscheduling/algorithms:column_generation",
                "@boost//:program_options",
        ],
        linkopts = select({
                    "@bazel_tools//src/conditions:windows": [],
                    "//conditions:default": ["-lpthread"],
        }),
)

cc_binary(
        name = "sosp2fsosp",
        srcs = ["sosp2fsosp.cpp"],
        deps = [
                "//starobservationschedulingsolver/flexiblestarobservationscheduling:flexiblestarobservationschedulingsolver",
                "@boost//:program_options",
        ],
)
