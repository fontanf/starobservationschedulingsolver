# Enable FetchContent.
include(FetchContent)

# Fetch fontanf/columngenerationsolver.
FetchContent_Declare(
    columngenerationsolver
    GIT_REPOSITORY https://github.com/fontanf/columngenerationsolver.git
    GIT_TAG 9c57300135f8d1ec2a44de4aeb53e0056d24f2f3
    #SOURCE_DIR "${PROJECT_SOURCE_DIR}/../columngenerationsolver/"
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(columngenerationsolver)
