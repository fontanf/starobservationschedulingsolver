#include "starobservationschedulingsolver/starobservationscheduling/instance_builder.hpp"

using namespace starobservationschedulingsolver::starobservationscheduling;

InstanceBuilder::InstanceBuilder(
        NightId number_of_nights,
        TargetId number_of_targets)
{
    instance_.observables_ = std::vector<std::vector<Observable>>(number_of_nights);
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
