#include "starobservationschedulingsolver/flexiblestarobservationscheduling/instance.hpp"

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/instance_builder.hpp"

#include "optimizationtools/utils/utils.hpp"
#include "optimizationtools/containers/indexed_set.hpp"
#include "optimizationtools/containers/indexed_map.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace starobservationschedulingsolver::flexiblestarobservationscheduling;

Instance::Instance(
        std::string instance_path,
        std::string format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexiblestarobservationschedulingsolver::Instance::Instance\n"
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

void Instance::read_catusse2016(
        std::ifstream& file)
{
    NightId number_of_nights;
    TargetId number_of_targets;
    std::string null;
    std::string line;

    std::getline(file, line);
    std::istringstream iss_m(line);
    iss_m >> null >> null >> number_of_nights;
    observables_ = std::vector<std::vector<Observable>>(number_of_nights);

    std::getline(file, line);
    std::istringstream iss_n(line);
    iss_n >> null >> null >> number_of_targets;

    InstanceBuilder instance_builder(number_of_nights, number_of_targets);

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
            ObservableId observable_id = instance_builder.add_observable(
                    night_id,
                    target_id,
                    release_date,
                    meridian,
                    deadline);
            for (Counter observation_time_pos = 0;
                    observation_time_pos < number_of_observation_times;
                    ++observation_time_pos) {
                instance_builder.add_observation_time(
                        night_id,
                        observable_id,
                        observation_times[observation_time_pos],
                        profits[observation_time_pos]);
            }
        }
    }

    *this = instance_builder.build();
}

std::ostream& Instance::print(
        std::ostream& os,
        int verbose) const
{
    if (verbose >= 1) {
        os
            << "Number of nights:       " << number_of_nights() << std::endl
            << "Number of targets:      " << number_of_targets() << std::endl
            << "Number of observables:  " << number_of_observables() << std::endl
            ;
    }

    if (verbose >= 2) {
        os << std::endl
            << std::setw(12) << "Night"
            << std::setw(12) << "Obs"
            << std::setw(12) << "Target"
            << std::setw(12) << "Rel. date"
            << std::setw(12) << "Meridian"
            << std::setw(12) << "Deadline"
            << std::endl
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
            << std::setw(12) << "--------"
            << std::setw(12) << "--------"
            << std::endl;
        for (NightId night_id = 0;
                night_id < number_of_nights();
                ++night_id) {
            for (ObservableId observable_id = 0;
                    observable_id < number_of_observables(night_id);
                    ++observable_id) {
                const Observable& observable = this->observable(night_id, observable_id);
                os
                    << std::setw(12) << night_id
                    << std::setw(12) << observable_id
                    << std::setw(12) << observable.target_id
                    << std::setw(12) << observable.release_date
                    << std::setw(12) << observable.meridian
                    << std::setw(12) << observable.deadline
                    << std::endl;
            }
        }
    }

    if (verbose >= 3) {
        os << std::endl
            << std::setw(12) << "Night"
            << std::setw(12) << "Obs"
            << std::setw(12) << "Target"
            << std::setw(12) << "Obs. time"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
            << std::setw(12) << "------"
            << std::endl;
        for (NightId night_id = 0;
                night_id < number_of_nights();
                ++night_id) {
            for (ObservableId observable_id = 0;
                    observable_id < number_of_observables(night_id);
                    ++observable_id) {
                const Observable& observable = this->observable(night_id, observable_id);
                for (Counter observation_time_pos = 0;
                        observation_time_pos < (Counter)observable.observation_times.size();
                        ++observation_time_pos) {
                    os
                        << std::setw(12) << night_id
                        << std::setw(12) << observable_id
                        << std::setw(12) << observable.target_id
                        << std::setw(12) << observable.observation_times[observation_time_pos]
                        << std::setw(12) << observable.profits[observation_time_pos]
                        << std::endl;
                }
            }
        }
    }

    return os;
}

void Instance::write(std::string instance_path) const
{
    if (instance_path.empty())
        return;
    std::ofstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexiblesinglenightstarobservationschedulingsolver::Instance::write\n"
                "Unable to open file \"" + instance_path + "\".");
    }

    std::vector<std::vector<std::pair<NightId, ObservableId>>> target_observables(number_of_targets());
    for (NightId night_id = 0;
            night_id < number_of_nights();
            ++night_id) {
        for (ObservableId observable_id = 0;
                observable_id < number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = this->observable(night_id, observable_id);
            target_observables[observable.target_id].push_back({night_id, observable_id});
        }
    }

    file << "Nb nights " << number_of_nights() << std::endl;
    file << "Nb obs " << number_of_targets() << std::endl;
    optimizationtools::IndexedMap<ObservableId> night2observable(number_of_nights(), -1);
    for (TargetId target_id = 0;
            target_id < number_of_targets();
            ++target_id) {

        night2observable.clear();
        for (auto p: target_observables[target_id])
            night2observable.set(p.first, p.second);

        file << "Obs " << target_id << std::endl;
        for (NightId night_id = 0;
                night_id < number_of_nights();
                ++night_id) {
            file << "Night " << night_id << ":";
            if (night2observable.contains(night_id)) {
                const Observable& observable = this->observable(night_id, night2observable[night_id]);
                file << " p " << observable.observation_times.size();
                for (Counter observation_time_pos = 0;
                        observation_time_pos < (Counter)observable.observation_times.size();
                        ++observation_time_pos) {
                    file << " " << observable.observation_times[observation_time_pos]
                        << " " << observable.profits[observation_time_pos];
                }
                file << " r " << observable.release_date
                    << " m " << observable.meridian
                    << " d " << observable.deadline;
            }
            file << std::endl;
        }
    }
}

std::pair<bool, Profit> Instance::check(
        std::string certificate_path,
        std::ostream& os,
        int verbose) const
{
    std::ifstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                "flexiblestarobservationschedulingsolver::Instance::check\n"
                "Unable to open file \"" + certificate_path + "\".");
    }

    if (verbose >= 2) {
        os << std::endl
            << std::setw(12) << "Night"
            << std::setw(12) << "Observable"
            << std::setw(12) << "Target"
            << std::setw(12) << "Time"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "-----"
            << std::setw(12) << "----------"
            << std::setw(12) << "------"
            << std::setw(12) << "----"
            << std::setw(12) << "------"
            << std::endl;
    }

    ObservableId current_night_number_of_observables = -1;
    optimizationtools::IndexedSet targets(number_of_targets());
    Profit profit = 0;
    TargetId number_of_duplicates = 0;
    TargetId number_of_deadline_violations = 0;
    for (NightId night_id = 0; night_id < number_of_nights(); ++night_id) {

        Time time = 0;

        file >> current_night_number_of_observables;
        for (ObservablePos observable_pos = 0;
                observable_pos < current_night_number_of_observables;
                ++observable_pos) {
            ObservableId observable_id;
            Counter observation_time_pos = -1;
            file >> observable_id >> observation_time_pos;
            const Observable& observable = this->observable(night_id, observable_id);

            // Check duplicates.
            if (targets.contains(observable.target_id)) {
                number_of_duplicates++;
                if (verbose >= 2) {
                    os << "Target " << observable.target_id
                        << " has already been observed." << std::endl;
                }
            }
            targets.add(observable.target_id);

            if (time < observable.release_date)
                time = observable.release_date;
            time += observable.observation_times[observation_time_pos];
            profit += observable.profits[observation_time_pos];

            if (verbose >= 2) {
                os
                    << std::setw(12) << night_id
                    << std::setw(12) << observable_id
                    << std::setw(12) << observable.target_id
                    << std::setw(12) << time
                    << std::setw(12) << profit
                    << std::endl;
            }

            // Check deadline.
            if (time > observable.deadline) {
                number_of_deadline_violations++;
                if (verbose >= 2) {
                    os << "Observation " << observable_id
                        << " ends after its deadline." << std::endl;
                }
            }

        }

    }

    bool feasible
        = (number_of_duplicates == 0)
        && (number_of_deadline_violations == 0);

    if (verbose >= 2)
        os << std::endl;
    if (verbose >= 1) {
        os
            << "Number of obsertions:           " << targets.size() << " / " << number_of_targets()  << std::endl
            << "Number of duplicates:           " << number_of_duplicates << std::endl
            << "Number of deadline violations:  " << number_of_deadline_violations << std::endl
            << "Feasible:                       " << feasible << std::endl
            << "Profit:                         " << profit << std::endl
            ;
    }
    return {feasible, profit};
}

void starobservationschedulingsolver::flexiblestarobservationscheduling::init_display(
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
        << "Flexible star observation scheduling problem" << std::endl
        << std::endl
        << "Instance" << std::endl
        << "--------" << std::endl;
    instance.print(info.os(), info.verbosity_level());
    info.os() << std::endl;
}
