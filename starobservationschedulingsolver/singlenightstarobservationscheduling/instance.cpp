#include "starobservationschedulingsolver/singlenightstarobservationscheduling/instance.hpp"

#include "optimizationtools/utils/utils.hpp"
#include "optimizationtools/containers/indexed_set.hpp"

using namespace starobservationschedulingsolver::singlenightstarobservationscheduling;

void Instance::add_target(
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
    targets_.push_back(target);

    total_profit_ += profit;
}

Instance::Instance(
        std::string instance_path,
        std::string format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "singlenightstarobservationschedulingsolver::Instance::Instance\n"
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
            << std::setw(12) << "Obs. time"
            << std::setw(12) << "Rel. date"
            << std::setw(12) << "Meirdian"
            << std::setw(12) << "Deadline"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
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
                << std::setw(12) << target.observation_time
                << std::setw(12) << target.release_date
                << std::setw(12) << target.meridian
                << std::setw(12) << target.deadline
                << std::setw(12) << target.profit
                << std::endl;
        }
    }

    return os;
}

void starobservationschedulingsolver::singlenightstarobservationscheduling::init_display(
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
        << "Single-night star observation scheduling problem" << std::endl
        << std::endl
        << "Instance" << std::endl
        << "--------" << std::endl;
    instance.print(info.os(), info.verbosity_level());
    info.os() << std::endl;
}
