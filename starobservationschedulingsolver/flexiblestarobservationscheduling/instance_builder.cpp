#include "starobservationschedulingsolver/flexiblestarobservationscheduling/instance_builder.hpp"

using namespace starobservationschedulingsolver::flexiblestarobservationscheduling;

InstanceBuilder::InstanceBuilder(
        NightId number_of_nights,
        TargetId number_of_targets)
{
    instance_.observables_ = std::vector<std::vector<Observable>>(number_of_nights);
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
