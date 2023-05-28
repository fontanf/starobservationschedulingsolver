def Settings(**kwargs):
    return {
            'flags': [
                '-x', 'c++',
                '-Wall', '-Wextra', '-Werror',
                '-I', '.',

                '-I', './bazel-starobservationschedulingsolver/external/'
                'json/single_include/',

                '-I', './bazel-starobservationschedulingsolver/external/'
                'googletest/googletest/include/',

                '-I', './bazel-starobservationschedulingsolver/external/'
                'boost/',

                # optimizationtools
                '-I', './bazel-starobservationschedulingsolver/external/'
                # '-I', './../'
                'optimizationtools/',

                # localsearchsolver
                '-I', './bazel-starobservationschedulingsolver/external/'
                # '-I', './../'
                'localsearchsolver/',

                # columngenerationsolver
                '-I', './bazel-starobservationschedulingsolver/external/'
                # '-I', './../'
                'columngenerationsolver/',

                # treesearchsolver
                '-I', './bazel-starobservationschedulingsolver/external/'
                # '-I', './../'
                'treesearchsolver/',
                ],
            }
