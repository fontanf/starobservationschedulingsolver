load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
load("//starobservationschedulingsolver:variables.bzl", "ALL_DEP", "ALL_COPTS")

cc_library(
        name = "column_generation",
        hdrs = ["column_generation.hpp"],
        srcs = ["column_generation.cpp"],
        deps = [
                "//flexiblestarobservationschedulingsolver:flexiblestarobservationschedulingsolver",
                "//flexiblesinglenightstarobservationschedulingsolver/algorithms:dynamic_programming",
                "@columngenerationsolver//columngenerationsolver:columngenerationsolver",
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
                "@boost//:program_options",
        ],
        visibility = ["//visibility:public"],
)
