load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
load("//starobservationschedulingsolver:variables.bzl", "ALL_DEP", "ALL_COPTS")

cc_library(
        name = "column_generation",
        hdrs = ["column_generation.hpp"],
        srcs = ["column_generation.cpp"],
        deps = [
                "//starobservationschedulingsolver/starobservationscheduling:starobservationschedulingsolver",
                "//starobservationschedulingsolver/singlenightstarobservationscheduling/algorithms:dynamic_programming",
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
                "//starobservationschedulingsolver/starobservationscheduling:starobservationschedulingsolver",
        ] + ALL_DEP,
        copts = ALL_COPTS,
        visibility = ["//visibility:public"],
)
