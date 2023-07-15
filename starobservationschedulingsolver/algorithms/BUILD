load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
load("//starobservationschedulingsolver:variables.bzl", "ALL_DEP", "ALL_COPTS")

cc_library(
        name = "column_generation",
        hdrs = ["column_generation.hpp"],
        srcs = ["column_generation.cpp"],
        deps = [
                "//starobservationschedulingsolver:starobservationschedulingsolver",
                "//singlenightstarobservationschedulingsolver/algorithms:dynamic_programming",
                "@columngenerationsolver//columngenerationsolver:columngenerationsolver",
        ] + ALL_DEP,
        copts = ALL_COPTS,
        visibility = ["//visibility:public"],
)

cc_library(
        name = "benders_decomposition",
        hdrs = ["benders_decomposition.hpp"],
        srcs = ["benders_decomposition.cpp"],
        deps = [
                "//starobservationschedulingsolver:starobservationschedulingsolver",
        ] + ALL_DEP,
        copts = ALL_COPTS,
        visibility = ["//visibility:public"],
)

cc_library(
        name = "algorithms",
        hdrs = ["algorithms.hpp"],
        srcs = ["algorithms.cpp"],
        deps = [
                "column_generation",
                "benders_decomposition",
                "@boost//:program_options",
        ],
        visibility = ["//visibility:public"],
)
