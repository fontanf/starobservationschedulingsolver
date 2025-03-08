#include "starobservationschedulingsolver/star_observation_scheduling/instance.hpp"

#include "optimizationtools/containers/indexed_set.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace starobservationschedulingsolver::star_observation_scheduling;

std::ostream& Instance::format(
        std::ostream& os,
        int verbosity_level) const
{
    if (verbosity_level >= 1) {
        os
            << "Number of nights:       " << number_of_nights() << std::endl
            << "Number of targets:      " << number_of_targets() << std::endl
            << "Number of observables:  " << number_of_observables() << std::endl
            ;
    }

    if (verbosity_level >= 2) {
        os << std::endl
            << std::setw(12) << "Target"
            << std::setw(12) << "Profit"
            << std::setw(12) << "# Obs"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "------"
            << std::setw(12) << "-----"
            << std::endl;
        for (TargetId target_id = 0;
                target_id < number_of_targets();
                ++target_id) {
            const Target& target = this->target(target_id);
            os
                << std::setw(12) << target_id
                << std::setw(12) << target.profit
                << std::setw(12) << target.observables.size()
                << std::endl;
        }
    }

    if (verbosity_level >= 2) {
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
            const Night& night = this->night(night_id);
            for (ObservableId observable_id = 0;
                    observable_id < (ObservableId)night.observables.size();
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
        const std::string& certificate_path,
        std::ostream& os,
        int verbosity_level) const
{
    std::ifstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                "starobservationschedulingsolver::Instance::check\n"
                "Unable to open file \"" + certificate_path + "\".");
    }

    if (verbosity_level >= 2) {
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
                if (verbosity_level >= 2) {
                    os << "Target " << observable.target_id
                        << " has already been observed." << std::endl;
                }
            }
            targets.add(observable.target_id);

            if (time < observable.release_date)
                time = observable.release_date;
            time += observable.observation_time;
            profit += this->target(observable.target_id).profit;

            if (verbosity_level >= 2) {
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
                if (verbosity_level >= 2) {
                    os << "Observation " << observable_id
                        << " ends after its deadline." << std::endl;
                }
            }

        }

    }

    bool feasible
        = (number_of_duplicates == 0)
        && (number_of_deadline_violations == 0);

    if (verbosity_level >= 2)
        os << std::endl;
    if (verbosity_level >= 1) {
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
