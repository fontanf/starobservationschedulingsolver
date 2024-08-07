add_library(StarObservationSchedulingSolver_flexible_star_observation_scheduling)
target_sources(StarObservationSchedulingSolver_flexible_star_observation_scheduling PRIVATE
    instance.cpp
    instance_builder.cpp
    solution.cpp
    utils.cpp
    algorithm_formatter.cpp)
target_include_directories(StarObservationSchedulingSolver_flexible_star_observation_scheduling PUBLIC
    ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(StarObservationSchedulingSolver_flexible_star_observation_scheduling PUBLIC
    StarObservationSchedulingSolver_star_observation_scheduling
    OptimizationTools::utils)
add_library(StarObservationSchedulingSolver::flexible_star_observation_scheduling ALIAS StarObservationSchedulingSolver_flexible_star_observation_scheduling)

add_subdirectory(algorithms)

add_executable(StarObservationSchedulingSolver_flexible_star_observation_scheduling_main)
target_sources(StarObservationSchedulingSolver_flexible_star_observation_scheduling_main PRIVATE
    main.cpp)
target_link_libraries(StarObservationSchedulingSolver_flexible_star_observation_scheduling_main PUBLIC
    StarObservationSchedulingSolver_flexible_star_observation_scheduling_column_generation
    Boost::program_options)
set_target_properties(StarObservationSchedulingSolver_flexible_star_observation_scheduling_main PROPERTIES OUTPUT_NAME "starobservationschedulingsolver_flexible_star_observation_scheduling")
install(TARGETS StarObservationSchedulingSolver_flexible_star_observation_scheduling_main)

add_executable(StarObservationSchedulingSolver_sosp2fsosp)
target_sources(StarObservationSchedulingSolver_sosp2fsosp PRIVATE
    sosp2fsosp.cpp)
target_link_libraries(StarObservationSchedulingSolver_sosp2fsosp PUBLIC
    StarObservationSchedulingSolver_flexible_star_observation_scheduling
    Boost::program_options)
set_target_properties(StarObservationSchedulingSolver_sosp2fsosp PROPERTIES OUTPUT_NAME "starobservationschedulingsolver_flexible_star_observation_scheduling")
install(TARGETS StarObservationSchedulingSolver_sosp2fsosp)
