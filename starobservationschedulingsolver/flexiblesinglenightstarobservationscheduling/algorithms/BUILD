cc_library(
        name = "dynamic_programming",
        hdrs = ["dynamic_programming.hpp"],
        srcs = ["dynamic_programming.cpp"],
        deps = [
                "//starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling:flexiblesinglenightstarobservationschedulingsolver",
        ],
        visibility = ["//visibility:public"],
)

cc_library(
        name = "algorithms",
        hdrs = ["algorithms.hpp"],
        srcs = ["algorithms.cpp"],
        deps = [
                ":dynamic_programming",
                "@boost//:program_options",
        ],
        visibility = ["//visibility:public"],
)
