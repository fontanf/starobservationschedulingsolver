#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/instance_builder.hpp"

using namespace starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling;

TargetId InstanceBuilder::add_target(
        Time release_date,
        Time meridian,
        Time deadline)
{
    Target target;
    target.release_date = release_date;
    target.meridian = meridian;
    target.deadline = deadline;
    instance_.targets_.push_back(target);

    return instance_.targets_.size() - 1;
}

void InstanceBuilder::add_observation_time(
        TargetId target_id,
        Time observation_time,
        Profit profit)
{
    instance_.targets_[target_id].observation_times.push_back(observation_time);
    instance_.targets_[target_id].profits.push_back(profit);
}


Instance InstanceBuilder::build()
{
    // Compute maximum_profit for each target and profit_sum_.
    for (TargetId target_id = 0;
            target_id < instance_.number_of_targets();
            ++target_id) {
        const Target& target = instance_.target(target_id);
        for (Time pos = 0;
                pos < (Time)target.observation_times.size();
                ++pos) {
            instance_.targets_[target_id].maximum_profit = std::max(
                    target.maximum_profit,
                    target.profits[pos]);
        }
        instance_.total_profit_ += target.maximum_profit;;
    }

    return std::move(instance_);
}
