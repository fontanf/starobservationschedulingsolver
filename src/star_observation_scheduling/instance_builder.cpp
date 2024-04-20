#include "starobservationschedulingsolver/star_observation_scheduling/instance_builder.hpp"

#include <fstream>
#include <sstream>

using namespace starobservationschedulingsolver::star_observation_scheduling;

void InstanceBuilder::set_number_of_nights(
        NightId number_of_nights)
{
    instance_.observables_ = std::vector<std::vector<Observable>>(number_of_nights);
}

void InstanceBuilder::set_number_of_targets(
        TargetId number_of_targets)
{
    instance_.profits_ = std::vector<Profit>(number_of_targets, 0);
}

void InstanceBuilder::add_observable(
        NightId night_id,
        TargetId target_id,
        Time release_date,
        Time meridian,
        Time deadline,
        Time observation_time)
{
    Observable observable;
    observable.target_id = target_id;
    observable.release_date = release_date;
    observable.meridian = meridian;
    observable.deadline = deadline;
    observable.observation_time = observation_time;
    instance_.observables_[night_id].push_back(observable);
}

void InstanceBuilder::set_profit(
        TargetId target_id,
        Profit profit)
{
    instance_.profits_[target_id] = profit;
}

void InstanceBuilder::read(
        const std::string& instance_path,
        const std::string& format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "Unable to open file \"" + instance_path + "\".");
    }

    if (format == ""
            || format == "default"
            || format == "catusse2016") {
        read_catusse2016(file);
    } else {
        throw std::invalid_argument(
                "Unknown instance format \"" + format + "\".");
    }
}

void InstanceBuilder::read_catusse2016(
        std::ifstream& file)
{
    TargetId number_of_targets;
    NightId number_of_nights;
    std::string null;
    std::string line;

    std::getline(file, line);
    std::istringstream iss_m(line);
    iss_m >> null >> null >> number_of_nights;

    std::getline(file, line);
    std::istringstream iss_n(line);
    iss_n >> null >> null >> number_of_targets;

    set_number_of_nights(number_of_nights);
    set_number_of_targets(number_of_targets);
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
        iss >> null >> null >> null >> null >> profit;
        set_profit(target_id, profit);

        for (NightId night_id = 0;
                night_id < number_of_nights;
                ++night_id) {
            std::getline(file, line);
            std::istringstream iss(line);
            iss >> null >> null >> null;
            if (iss.eof())
                continue;
            iss >> observation_time
                >> null >> release_date
                >> null >> meridian
                >> null >> deadline;
            add_observable(
                    night_id,
                    target_id,
                    release_date,
                    meridian,
                    deadline,
                    observation_time);
        }
    }
}

Instance InstanceBuilder::build()
{
    // Compute number_of_observables_.
    for (NightId night_id = 0;
            night_id < instance_.number_of_nights();
            ++night_id) {
        instance_.number_of_observables_ += instance_.observables_[night_id].size();
    }

    // Compute profit_sum_.
    for (TargetId target_id = 0;
            target_id < instance_.number_of_targets();
            ++target_id) {
        instance_.profit_sum_ += instance_.profit(target_id);
    }

    return std::move(instance_);
}
