#include "starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/instance.hpp"

#include <iomanip>

using namespace starobservationschedulingsolver::flexible_single_night_star_observation_scheduling;

std::ostream& Instance::format(
        std::ostream& os,
        int verbosity_level) const
{
    if (verbosity_level >= 1) {
        os
            << "Number of targets:  " << number_of_targets() << std::endl
            ;
    }

    if (verbosity_level >= 2) {
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

    if (verbosity_level >= 3) {
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
