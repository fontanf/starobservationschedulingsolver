#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/instance.hpp"

#include "optimizationtools/utils/utils.hpp"
#include "optimizationtools/containers/indexed_set.hpp"

using namespace starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling;

TargetId Instance::add_target(
        Time release_date,
        Time meridian,
        Time deadline)
{
    Target target;
    target.release_date = release_date;
    target.meridian = meridian;
    target.deadline = deadline;
    targets_.push_back(target);

    return targets_.size() - 1;
}

void Instance::add_observation_time(
        TargetId target_id,
        Time observation_time,
        Profit profit)
{
    targets_[target_id].observation_times.push_back(observation_time);
    targets_[target_id].profits.push_back(profit);
}

Instance::Instance(
        std::string instance_path,
        std::string format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexiblesinglenightstarobservationschedulingsolver::Instance::Instance\n"
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

void Instance::read_default(std::ifstream& file)
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

std::ostream& Instance::print(
        std::ostream& os,
        int verbose) const
{
    if (verbose >= 1) {
        os
            << "Number of targets:  " << number_of_targets() << std::endl
            ;
    }

    if (verbose >= 2) {
        os << std::endl
            << std::setw(12) << "Target"
            << std::setw(12) << "Rel. date"
            << std::setw(12) << "Meridian"
            << std::setw(12) << "Deadline"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
            << std::setw(12) << "--------"
            << std::setw(12) << "--------"
            << std::setw(12) << "------"
            << std::endl;
        for (TargetId target_id = 0;
                target_id < number_of_targets();
                ++target_id) {
            const Target& target = this->target(target_id);
            os
                << std::setw(12) << target_id
                << std::setw(12) << target.release_date
                << std::setw(12) << target.deadline
                << std::endl;
        }
    }

    if (verbose >= 3) {
        os << std::endl
            << std::setw(12) << "Target"
            << std::setw(12) << "Obs. time"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
            << std::setw(12) << "------"
            << std::endl;
        for (TargetId target_id = 0;
                target_id < number_of_targets();
                ++target_id) {
            const Target& target = this->target(target_id);
            for (Counter observation_time_pos = 0;
                    observation_time_pos < (Counter)target.observation_times.size();
                    ++observation_time_pos) {
                os
                    << std::setw(12) << target_id
                    << std::setw(12) << target.observation_times[observation_time_pos]
                    << std::setw(12) << target.profits[observation_time_pos]
                    << std::endl;
            }
        }
    }

    return os;
}

void starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::init_display(
        const Instance& instance,
        optimizationtools::Info& info)
{
    info.os()
        << "=======================================" << std::endl
        << "    StarObservationSchedulingSolver    " << std::endl
        << "=======================================" << std::endl
        << std::endl
        << "Problem" << std::endl
        << "-------" << std::endl
        << "Flexible single-night star observation scheduling problem" << std::endl
        << std::endl
        << "Instance" << std::endl
        << "--------" << std::endl;
    instance.print(info.os(), info.verbosity_level());
    info.os() << std::endl;
}
