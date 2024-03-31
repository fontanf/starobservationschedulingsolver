load("//starobservationschedulingsolver:variables.bzl", "STDCPP")
load("//starobservationschedulingsolver:variables.bzl", "ALL_DEP", "ALL_COPTS")

cc_library(
        name = "column_generation",
        hdrs = ["column_generation.hpp"],
        srcs = ["column_generation.cpp"],
        deps = [
                "//starobservationschedulingsolver/flexible_star_observation_scheduling:flexible_star_observation_scheduling",
                "//starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/algorithms:dynamic_programming",
                "@columngenerationsolver//columngenerationsolver:columngenerationsolver",
        ] + ALL_DEP,
        copts = ALL_COPTS,
        visibility = ["//visibility:public"],
)
