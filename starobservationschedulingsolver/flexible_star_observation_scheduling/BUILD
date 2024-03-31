cc_library(
        name = "flexible_star_observation_scheduling",
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
                "//starobservationschedulingsolver/star_observation_scheduling:star_observation_scheduling",
                "@optimizationtools//optimizationtools/utils:utils",
                "@optimizationtools//optimizationtools/containers:containers",
        ],
        visibility = ["//visibility:public"],
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [
                "//starobservationschedulingsolver/flexible_star_observation_scheduling/algorithms:column_generation",
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
                "//starobservationschedulingsolver/flexible_star_observation_scheduling:flexible_star_observation_scheduling",
                "@boost//:program_options",
        ],
)
