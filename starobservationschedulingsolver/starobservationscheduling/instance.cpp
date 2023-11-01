#include "starobservationschedulingsolver/starobservationscheduling/instance.hpp"

#include "optimizationtools/utils/utils.hpp"
#include "optimizationtools/containers/indexed_set.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace starobservationschedulingsolver::starobservationscheduling;

void Instance::add_observable(
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
    observables_[night_id].push_back(observable);

    number_of_observables_++;
}

void Instance::set_profit(
        TargetId target_id,
        Profit profit)
{
    profit_sum_ -= profits_[target_id];
    profits_[target_id] = profit;
    profit_sum_ += profits_[target_id];
}

Instance::Instance(
        std::string instance_path,
        std::string format)
{
    std::ifstream file(instance_path);
    if (!file.good()) {
        throw std::runtime_error(
                "starobservationschedulingsolver::Instance::Instance\n"
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

void Instance::read_catusse2016(std::ifstream& file)
{
    TargetId number_of_targets;
    NightId number_of_nights;
    std::string null;
    std::string line;

    std::getline(file, line);
    std::istringstream iss_m(line);
    iss_m >> null >> null >> number_of_nights;
    observables_ = std::vector<std::vector<Observable>>(number_of_nights);

    std::getline(file, line);
    std::istringstream iss_n(line);
    iss_n >> null >> null >> number_of_targets;
    profits_ = std::vector<Profit>(number_of_targets);

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
            << std::setw(12) << "Target"
            << std::setw(12) << "Profit"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "------"
            << std::endl;
        for (TargetId target_id = 0;
                target_id < number_of_targets();
                ++target_id) {
            os
                << std::setw(12) << target_id
                << std::setw(12) << profit(target_id)
                << std::endl;
        }
    }

    if (verbose >= 2) {
        os << std::endl
            << std::setw(12) << "Night"
            << std::setw(12) << "Obs"
            << std::setw(12) << "Target"
            << std::setw(12) << "Obs. time"
            << std::setw(12) << "Rel. date"
            << std::setw(12) << "Meridian"
            << std::setw(12) << "Deadline"
            << std::endl
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::setw(12) << "------"
            << std::setw(12) << "---------"
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
                    << std::setw(12) << observable.observation_time
                    << std::setw(12) << observable.release_date
                    << std::setw(12) << observable.meridian
                    << std::setw(12) << observable.deadline
                    << std::endl;
            }
        }
    }

    return os;
}

std::pair<bool, Profit> Instance::check(
        std::string certificate_path,
        std::ostream& os,
        int verbose) const
{
    std::ifstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                "starobservationschedulingsolver::Instance::check\n"
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
    std::string tmp;
    for (NightId night_id = 0; night_id < number_of_nights(); ++night_id) {

        Time time = 0;

        file >> current_night_number_of_observables;
        for (ObservablePos observable_pos = 0;
                observable_pos < current_night_number_of_observables;
                ++observable_pos) {
            ObservableId observable_id;
            file >> observable_id >> tmp >> tmp >> tmp;
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
            time += observable.observation_time;
            profit += this->profit(observable.target_id);

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

void starobservationschedulingsolver::starobservationscheduling::init_display(
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
        << "Star observation scheduling problem" << std::endl
        << std::endl
        << "Instance" << std::endl
        << "--------" << std::endl;
    instance.print(info.os(), info.verbosity_level());
    info.os() << std::endl;
}
