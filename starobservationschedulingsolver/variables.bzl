STDCPP = select({
            "@bazel_tools//src/conditions:windows": ['/std:c++latest'],
            "//conditions:default": ["-std=c++11"],})

CLP_COPTS = select({
            "//starobservationschedulingsolver:clp_build": ["-DCLP_FOUND"],
            "//conditions:default": []})
CLP_DEP = select({
            "//starobservationschedulingsolver:clp_windows": ["@clp_windows//:clp"],
            "//conditions:default": []
        }) + select({
            "//starobservationschedulingsolver:clp_linux": ["@clp_linux//:clp"],
            "//conditions:default": []})

CPLEX_COPTS = select({
            "//starobservationschedulingsolver:cplex_build": [
                    "-DCPLEX_FOUND",
                    "-m64",
                    "-DIL_STD"],
            "//conditions:default": []})
CPLEX_DEP = select({
            "//starobservationschedulingsolver:cplex_build": ["@cplex//:cplex"],
            "//conditions:default": []})

XPRESS_COPTS = select({
            "//starobservationschedulingsolver:xpress_build": ["-DXPRESS_FOUND"],
            "//conditions:default": []})
XPRESS_DEP = select({
            "//starobservationschedulingsolver:xpress_build": ["@xpress//:xpress"],
            "//conditions:default": []})

ALL_COPTS = CLP_COPTS + XPRESS_COPTS + CPLEX_COPTS
ALL_DEP = CLP_DEP + XPRESS_DEP + CPLEX_DEP
