#include "starobservationschedulingsolver/single_night_star_observation_scheduling/solution.hpp"

#include "optimizationtools/utils/utils.hpp"

using namespace starobservationschedulingsolver::single_night_star_observation_scheduling;

Solution::Solution(
        const Instance& instance):
    instance_(&instance),
    targets_(instance.number_of_targets(), 0)
{  }

void Solution::append_observation(
        TargetId target_id,
        Time start_time)
{
    const Target& target = instance().target(target_id);
    if (target.release_date > start_time) {
        throw std::runtime_error(
                "target.release_date > start_time");
    }
    if (target.deadline < start_time + target.observation_time) {
        throw std::runtime_error(
                "target.deadline < start_time + target.observation_time");
    }
    if (start_time < current_time_) {
        throw std::runtime_error(
                "start_time < current_time_");
    }
    if (targets_[target_id] == 1) {
        throw std::runtime_error(
                "targets_[target_id] == 1");
    }

    Observation observation;
    observation.target_id = target_id;
    observation.start_time = start_time;
    observations_.push_back(observation);

    targets_[target_id] = 1;
    current_time_ = start_time + target.observation_time;
    profit_ += instance().target(target_id).profit;
}

std::ostream& Solution::format(
        std::ostream& os,
        int verbosity_level) const
{
    if (verbosity_level >= 1) {
        os
            << "Number of targets:  " << optimizationtools::Ratio<TargetId>(number_of_observations(), instance().number_of_targets()) << std::endl
            << "Feasible:           " << feasible() << std::endl
            << "Profit:             " << optimizationtools::Ratio<Profit>(profit(), instance().total_profit()) << std::endl
            ;
    }

    if (verbosity_level >= 2) {
        os << std::endl
            << std::setw(12) << "Target"
            << std::setw(12) << "Start"
            << std::setw(12) << "End"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::endl;
        for (TargetId observation_pos = 0;
                observation_pos < number_of_observations();
                ++observation_pos) {
            const Observation& observation = this->observation(observation_pos);
            const Target& target = instance().target(observation.target_id);
            os
                << std::setw(12) << observation.target_id
                << std::setw(12) << observation.start_time
                << std::setw(12) << observation.start_time + target.observation_time
                << std::endl;
        }
    }

    return os;
}

nlohmann::json Solution::to_json() const
{
    return nlohmann::json {
        {"NumberOfTargets", number_of_observations()},
        {"Feasible", feasible()},
        {"Profit", profit()},
    };
}

void Solution::write(
        const std::string& certificate_path) const
{
    if (certificate_path.empty())
        return;
    std::ofstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                "single_night_star_observation_scheduling::Solution::write\n"
                "Unable to open file \"" + certificate_path + "\".");
    }

    file << observations_.size() << std::endl;
    for (const auto& observation: observations_)
        file << " " << observation.target_id;
    file << std::endl;
}
