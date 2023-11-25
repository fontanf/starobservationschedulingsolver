#include "starobservationschedulingsolver/singlenightstarobservationscheduling/instance_builder.hpp"

using namespace starobservationschedulingsolver::singlenightstarobservationscheduling;

void InstanceBuilder::add_target(
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
    instance_.targets_.push_back(target);
}

Instance InstanceBuilder::build()
{
    // Compute profit_sum_.
    for (TargetId target_id = 0;
            target_id < instance_.number_of_targets();
            ++target_id) {
        instance_.total_profit_ += instance_.target(target_id).profit;
    }

    return std::move(instance_);
}
