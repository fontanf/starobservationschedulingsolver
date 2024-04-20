#include "starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/instance_builder.hpp"

#include <fstream>
#include <sstream>

using namespace starobservationschedulingsolver::flexible_single_night_star_observation_scheduling;

TargetId InstanceBuilder::add_target(
        Time release_date,
        Time meridian,
        Time deadline)
{
    Target target;
    target.release_date = release_date;
    target.meridian = meridian;
    target.deadline = deadline;
    instance_.targets_.push_back(target);

    return instance_.targets_.size() - 1;
}

void InstanceBuilder::add_observation_time(
        TargetId target_id,
        Time observation_time,
        Profit profit)
{
    instance_.targets_[target_id].observation_times.push_back(observation_time);
    instance_.targets_[target_id].profits.push_back(profit);
}

void InstanceBuilder::read(
        const std::string& instance_path,
        const std::string& format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexible_single_night_star_observation_scheduling::Instance::Instance\n"
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

    std::vector<Time> observation_times;
    std::vector<Profit> profits;
    Time observation_time = -1;
    Profit profit = -1;
    Counter number_of_observation_times = -1;
    Time release_date = -1;
    Time meridian = -1;
    Time deadline = -1;
    for (TargetId target_id = 0;
            target_id < number_of_targets;
            ++target_id) {
        std::getline(file, line);
        std::istringstream iss(line);
        observation_times.clear();
        profits.clear();
        iss >> null >> null
            >> null >> number_of_observation_times;
        for (Counter observation_time_pos = 0;
                observation_time_pos < number_of_observation_times;
                ++observation_time_pos) {
            iss >> observation_time >> profit;
            observation_times.push_back(observation_time);
            profits.push_back(profit);
        }
        iss >> null >> release_date
            >> null >> meridian
            >> null >> deadline;
        add_target(
                    release_date,
                    meridian,
                    deadline);
        for (Counter observation_time_pos = 0;
                observation_time_pos < number_of_observation_times;
                ++observation_time_pos) {
            add_observation_time(
                    target_id,
                    observation_times[observation_time_pos],
                    profits[observation_time_pos]);
        }
    }
}

Instance InstanceBuilder::build()
{
    // Compute maximum_profit for each target and profit_sum_.
    for (TargetId target_id = 0;
            target_id < instance_.number_of_targets();
            ++target_id) {
        const Target& target = instance_.target(target_id);
        for (Time pos = 0;
                pos < (Time)target.observation_times.size();
                ++pos) {
            instance_.targets_[target_id].maximum_profit = std::max(
                    target.maximum_profit,
                    target.profits[pos]);
        }
        instance_.total_profit_ += target.maximum_profit;;
    }

    return std::move(instance_);
}
