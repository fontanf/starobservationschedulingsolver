#include "starobservationschedulingsolver/flexible_star_observation_scheduling/solution.hpp"

#include "optimizationtools/utils/utils.hpp"

using namespace starobservationschedulingsolver::flexible_star_observation_scheduling;

Solution::Solution(
        const Instance& instance):
    instance_(&instance),
    targets_(instance.number_of_targets(), 0),
    nights_(instance.number_of_nights())
{  }

void Solution::append_observation(
        NightId night_id,
        ObservableId observable_id,
        Counter observation_time_pos,
        Time start_time)
{
    const Observable& observable = instance().observable(night_id, observable_id);
    Time observation_time = observable.observation_times[observation_time_pos];
    SolutionNight& night = nights_[night_id];
    if (observable.release_date > start_time) {
        throw std::runtime_error(
                "observable.release_date > start_time");
    }
    if (start_time + observation_time > observable.deadline) {
        throw std::runtime_error(
                "start_time + observation_time > observable.deadline");
    }
    if (start_time < night.current_time) {
        throw std::runtime_error(
                "start_time < current_time_");
    }
    if (targets_[observable.target_id] == 1) {
        throw std::runtime_error(
                "observables_[observable_id] == 1");
    }
    if (start_time > observable.meridian) {
        throw std::runtime_error(
                "start_time > target.meridian");
    }
    if (start_time + observation_time < observable.meridian) {
        throw std::runtime_error(
                "start_time + observation_time < target.meridian");
    }

    Observation observation;
    observation.observable_id = observable_id;
    observation.start_time = start_time;
    observation.observation_time_pos = observation_time_pos;
    night.observations.push_back(observation);

    night.current_time = start_time + observation_time;
    targets_[observable.target_id] = 1;
    number_of_observations_++;
    profit_ += observable.profits[observation_time_pos];
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
            << std::setw(12) << "Night"
            << std::setw(12) << "Target"
            << std::setw(12) << "ObsPos"
            << std::setw(12) << "Start"
            << std::setw(12) << "End"
            << std::endl
            << std::setw(12) << "-----"
            << std::setw(12) << "------"
            << std::setw(12) << "------"
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::endl;
        for (NightId night_id = 0;
                night_id < instance().number_of_nights();
                ++night_id) {
            const SolutionNight& night = this->night(night_id);
            for (TargetId observation_pos = 0;
                    observation_pos < (ObservableId)night.observations.size();
                    ++observation_pos) {
                const Observation& observation = night.observations[observation_pos];
                const Observable& observable = instance().observable(night_id, observation.observable_id);
                os
                    << std::setw(12) << night_id
                    << std::setw(12) << observable.target_id
                    << std::setw(12) << observation.observation_time_pos
                    << std::setw(12) << observation.start_time
                    << std::setw(12) << observation.start_time + observable.observation_times[observation.observation_time_pos]
                    << std::endl;
            }
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
                "flexible_star_observation_scheduling::Solution::write\n"
                "Unable to open file \"" + certificate_path + "\".");
    }

    file << instance().number_of_nights() << std::endl;
    for (NightId night_id = 0;
            night_id < instance().number_of_nights();
            ++night_id) {
        const SolutionNight& night = this->night(night_id);
        file << night.observations.size() << std::endl;
        for (const auto& observation: night.observations) {
            const Observable& observable = instance().observable(night_id, observation.observable_id);
            file << observation.observable_id
                << " " << instance().observable(night_id, observation.observable_id).target_id
                << " " << observation.start_time
                << " " << observation.start_time + observable.observation_times[observation.observation_time_pos]
                << " " << observable.profits[observation.observation_time_pos] / observable.maximum_profit
                << std::endl;
        }
    }
}
