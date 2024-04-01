import sys
import os

test_results_directory = sys.argv[1]

star_observation_scheduling_data = [
        os.path.join("catusse2016", "ins_400_71_1"),
        os.path.join("catusse2016", "ins_400_71_2"),
        os.path.join("catusse2016", "ins_400_71_3"),
        os.path.join("catusse2016", "ins_400_71_4"),
        os.path.join("catusse2016", "ins_400_71_5"),
        os.path.join("catusse2016", "ins_600_107_1"),
        os.path.join("catusse2016", "ins_600_107_2"),
        os.path.join("catusse2016", "ins_600_107_3"),
        os.path.join("catusse2016", "ins_600_107_4"),
        os.path.join("catusse2016", "ins_600_107_5"),
        os.path.join("catusse2016", "real.txt")]
star_observation_scheduling_main = os.path.join(
        "bazel-bin",
        "starobservationschedulingsolver",
        "star_observation_scheduling",
        "main")
for instance in star_observation_scheduling_data:
    instance_path = os.path.join(
            "data",
            "star_observation_scheduling",
            instance)
    json_output_path = os.path.join(
            test_results_directory,
            "star_observation_scheduling",
            instance + ".json")
    if not os.path.exists(os.path.dirname(json_output_path)):
        os.makedirs(os.path.dirname(json_output_path))
    command = (
            star_observation_scheduling_main
            + "  --verbosity-level 1"
            + "  --input \"" + instance_path + "\""
            + "  --algorithm column-generation"
            + "  --output \"" + json_output_path + "\"")
    print(command)
    os.system(command)
    print()

flexible_star_observation_scheduling_data = [
        os.path.join("catusse2016_discrete_80", "ins_400_71_1"),
        os.path.join("catusse2016_discrete_80", "ins_400_71_2"),
        os.path.join("catusse2016_discrete_80", "ins_400_71_3"),
        os.path.join("catusse2016_discrete_80", "ins_400_71_4"),
        os.path.join("catusse2016_discrete_80", "ins_400_71_5"),
        os.path.join("catusse2016_discrete_85", "ins_400_71_1"),
        os.path.join("catusse2016_discrete_85", "ins_400_71_2"),
        os.path.join("catusse2016_discrete_85", "ins_400_71_3"),
        os.path.join("catusse2016_discrete_85", "ins_400_71_4"),
        os.path.join("catusse2016_discrete_85", "ins_400_71_5")]
flexible_star_observation_scheduling_main = os.path.join(
        "bazel-bin",
        "starobservationschedulingsolver",
        "flexible_star_observation_scheduling",
        "main")
for instance in flexible_star_observation_scheduling_data:
    instance_path = os.path.join(
            "data",
            "flexible_star_observation_scheduling",
            instance)
    json_output_path = os.path.join(
            test_results_directory,
            "flexible_star_observation_scheduling",
            instance)
    if not os.path.exists(os.path.dirname(json_output_path)):
        os.makedirs(os.path.dirname(json_output_path))
    command = (
            flexible_star_observation_scheduling_main
            + "  --verbosity-level 1"
            + "  --input \"" + instance_path + "\""
            + "  --algorithm column-generation"
            + "  --output \"" + json_output_path + "\"")
    print(command)
    os.system(command)
    print()
