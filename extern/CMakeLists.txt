# Enable FetchContent.
include(FetchContent)

# Fetch fontanf/columngenerationsolver.
FetchContent_Declare(
    columngenerationsolver
    GIT_REPOSITORY https://github.com/fontanf/columngenerationsolver.git
    GIT_TAG 32561f17c6fa7100b2dedb5d397dab1a03c9db2c)
    #SOURCE_DIR "${PROJECT_SOURCE_DIR}/../columngenerationsolver/")
FetchContent_MakeAvailable(columngenerationsolver)
