add_library(StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation)
target_sources(StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation PRIVATE
    column_generation.cpp)
target_include_directories(StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation PUBLIC
    ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation PUBLIC
    StarObservationSchedulingSolver_flexible_star_observation_scheduling
    StarObservationSchedulingSolver_flexible_single_night_star_observation_scheduling_dynamic_programming
    ColumnGenerationSolver::columngenerationsolver)
add_library(StarObservationSchedulingSolver::flexible_star_observation_scheduling::column_generation ALIAS StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation)
