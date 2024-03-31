#include "starobservationschedulingsolver/single_night_star_observation_scheduling/instance_builder.hpp"

#include <fstream>
#include <sstream>

using namespace starobservationschedulingsolver::single_night_star_observation_scheduling;

void InstanceBuilder::add_target(
        Time release_date,
        Time meridian,
        Time deadline,
        Time observation_time,
        Profit profit)
{
    Target target;
    target.release_date = release_date;
    target.meridian = meridian,
    target.deadline = deadline;
    target.observation_time = observation_time;
    target.profit = profit;
    instance_.targets_.push_back(target);
}

void InstanceBuilder::read(
        const std::string& instance_path,
        const std::string& format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "single_night_star_observation_scheduling::Instance::Instance\n"
                "Unable to open file \"" + instance_path + "\".");
    }

    if (format == ""
            || format == "default") {
        read_default(file);
    } else {
        throw std::invalid_argument(
                "Unknown instance format \"" + format + "\".");
    }
    file.close();
}

void InstanceBuilder::read_default(
        std::ifstream& file)
{
    TargetId number_of_targets;
    std::string null;
    std::string line;

    std::getline(file, line);

    std::getline(file, line);
    std::istringstream iss_n(line);
    iss_n >> null >> null >> number_of_targets;

    Profit profit = -1;
    Time observation_time = -1;
    Time release_date = -1;
    Time meridian = -1;
    Time deadline = -1;
    for (TargetId target_id = 0;
            target_id < number_of_targets;
            ++target_id) {
        std::getline(file, line);
        std::istringstream iss(line);
        iss >> null >> null >> null >> null >> profit
            >> null >> observation_time
            >> null >> release_date
            >> null >> meridian
            >> null >> deadline;
        add_target(
                    release_date,
                    meridian,
                    deadline,
                    observation_time,
                    profit);
    }
}

Instance InstanceBuilder::build()
{
    // Compute profit_sum_.
    for (TargetId target_id = 0;
            target_id < instance_.number_of_targets();
            ++target_id) {
        instance_.total_profit_ += instance_.target(target_id).profit;
    }

    return std::move(instance_);
}
