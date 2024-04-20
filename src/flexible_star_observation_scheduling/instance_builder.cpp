#include "starobservationschedulingsolver/flexible_star_observation_scheduling/instance_builder.hpp"

#include <fstream>
#include <sstream>

using namespace starobservationschedulingsolver::flexible_star_observation_scheduling;

void InstanceBuilder::set_number_of_nights(
        NightId number_of_nights)
{
    instance_.observables_ = std::vector<std::vector<Observable>>(number_of_nights);
}

void InstanceBuilder::set_number_of_targets(
        TargetId number_of_targets)
{
    instance_.number_of_targets_ = number_of_targets;
}

ObservableId InstanceBuilder::add_observable(
        NightId night_id,
        TargetId target_id,
        Time release_date,
        Time meridian,
        Time deadline)
{
    Observable observable;
    observable.target_id = target_id;
    observable.release_date = release_date;
    observable.meridian = meridian;
    observable.deadline = deadline;
    instance_.observables_[night_id].push_back(observable);

    return instance_.observables_[night_id].size() - 1;
}

void InstanceBuilder::add_observation_time(
        NightId night_id,
        ObservableId observable_id,
        Time observation_time,
        Profit profit)
{
    instance_.observables_[night_id][observable_id].observation_times.push_back(observation_time);
    instance_.observables_[night_id][observable_id].profits.push_back(profit);
}

void InstanceBuilder::read(
        const std::string& instance_path,
        const std::string& format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexible_star_observation_scheduling::Instance::Instance\n"
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
    file.close();
}

void InstanceBuilder::read_catusse2016(
        std::ifstream& file)
{
    NightId number_of_nights;
    TargetId number_of_targets;
    std::string null;
    std::string line;

    std::getline(file, line);
    std::istringstream iss_m(line);
    iss_m >> null >> null >> number_of_nights;
    set_number_of_nights(number_of_nights);

    std::getline(file, line);
    std::istringstream iss_n(line);
    iss_n >> null >> null >> number_of_targets;
    set_number_of_targets(number_of_targets);

    std::vector<Time> observation_times;
    std::vector<Profit> profits;
    Counter number_of_observation_times = -1;
    Time observation_time = -1;
    Profit profit = -1;
    Time release_date = -1;
    Time meridian = -1;
    Time deadline = -1;
    for (TargetId target_id = 0;
            target_id < number_of_targets;
            ++target_id) {
        std::getline(file, line);
        std::istringstream iss(line);
        iss >> null >> null;

        for (NightId night_id = 0;
                night_id < number_of_nights;
                ++night_id) {
            std::getline(file, line);
            std::istringstream iss(line);
            iss >> null >> null >> null;
            if (iss.eof())
                continue;
            observation_times.clear();
            profits.clear();
            iss >> number_of_observation_times;
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
            ObservableId observable_id = add_observable(
                    night_id,
                    target_id,
                    release_date,
                    meridian,
                    deadline);
            for (Counter observation_time_pos = 0;
                    observation_time_pos < number_of_observation_times;
                    ++observation_time_pos) {
                add_observation_time(
                        night_id,
                        observable_id,
                        observation_times[observation_time_pos],
                        profits[observation_time_pos]);
            }
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

    // Compute maximum_profit for each observable and profit_sum_.
    for (NightId night_id = 0;
            night_id < instance_.number_of_nights();
            ++night_id) {
        for (ObservableId observable_id = 0;
                observable_id < instance_.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance_.observable(night_id, observable_id);
            for (Time pos = 0;
                    pos < (Time)observable.observation_times.size();
                    ++pos) {
                instance_.observables_[night_id][observable_id].maximum_profit = std::max(
                        observable.maximum_profit,
                        observable.profits[pos]);
            }
            instance_.profit_sum_ += observable.maximum_profit;;
        }
    }

    return std::move(instance_);
}
