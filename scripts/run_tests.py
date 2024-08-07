import argparse
import sys
import os

parser = argparse.ArgumentParser(description='')
parser.add_argument('directory')
parser.add_argument(
        "-t", "--tests",
        type=str,
        nargs='*',
        help='')

args = parser.parse_args()


star_observation_scheduling_data = os.environ['STAR_OBSERVATION_SCHEDULING_DATA']
star_observation_scheduling_main = os.path.join(
        "install",
        "bin",
        "starobservationschedulingsolver_star_observation_scheduling")


if args.tests is None or "star-observation-scheduling-column-generation" in args.tests:
    print("Star observation scheduling problem / column generation")
    print("-------------------------------------------------------")
    print()

    data = [
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
    for instance in data:
        instance_path = os.path.join(
                star_observation_scheduling_data,
                instance)
        json_output_path = os.path.join(
                args.directory,
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
        status = os.system(command)
        if status != 0:
            sys.exit(1)
        print()
    print()
    print()


flexible_star_observation_scheduling_data = os.environ['FLEXIBLE_STAR_OBSERVATION_SCHEDULING_DATA']
flexible_star_observation_scheduling_main = os.path.join(
        "install",
        "bin",
        "starobservationschedulingsolver_flexible_star_observation_scheduling")


if args.tests is None or "flexible-star-observation-scheduling-column-generation" in args.tests:
    print("Flexible star observation scheduling / column generation")
    print("--------------------------------------------------------")
    print()

    data = [
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
    for instance in data:
        instance_path = os.path.join(
                flexible_star_observation_scheduling_data,
                instance)
        json_output_path = os.path.join(
                args.directory,
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
        status = os.system(command)
        if status != 0:
            sys.exit(1)
        print()
    print()
    print()
