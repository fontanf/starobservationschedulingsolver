#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace starobservationschedulingsolver
{
namespace flexible_star_observation_scheduling
{

using NightId = int64_t;
using TargetId = int64_t;
using ObservableId = int64_t;
using ObservablePos = int64_t;
using Profit = double;
using Time = int64_t;
using Counter = int64_t;
using Seed = int64_t;

/**
 * Structure for an observable.
 */
struct Observable
{
    /** Target of the observable. */
    TargetId target_id;

    /** Release date of the observable. */
    Time release_date;

    /** Meridian of the observable. */
    Time meridian;

    /** Deadline of the observable. */
    Time deadline;

    /** Observation times of the observable. */
    std::vector<Time> observation_times;

    /** Profits of the observation. */
    std::vector<Profit> profits;

    /** Maximum profit. */
    Profit maximum_profit = 0;
};

/**
 * Instance class for a 'flexible_star_observation_scheduling' problem.
 */
class Instance
{

public:

    /*
     * Getters
     */

    /** Get the number of nights. */
    NightId number_of_nights() const { return observables_.size(); }

    /** Get the number of targets. */
    TargetId number_of_targets() const { return number_of_targets_; }

    /** Get the total number of observables. */
    ObservableId number_of_observables() const { return number_of_observables_; }

    /** Get the number of observable in a given night. */
    ObservableId number_of_observables(NightId night_id) const { return observables_[night_id].size(); }

    /** Get an observble. */
    const Observable& observable(
            NightId night_id,
            ObservableId observable_id) const
    {
        return observables_[night_id][observable_id];
    }

    /** Get the total profit of the targets. */
    Profit total_profit() const { return profit_sum_; }

    /*
     * Export
     */

    /** Print the instance. */
    std::ostream& format(
            std::ostream& os,
            int verbosity_level = 1) const;

    /** Write the instance to a file. */
    void write(
            const std::string& instance_path) const;

    /*
     * Checkers
     */

    /** Check a certificate. */
    std::pair<bool, Profit> check(
            const std::string& certificate_path,
            std::ostream& os,
            int verbosity_level = 1) const;

private:

    /*
     * Private methods
     */

    /** Create an instance manually. */
    Instance() { }

    /*
     * Private attributes
     */

    /** Number of targets. */
    TargetId number_of_targets_ = 0;

    /** Observables. */
    std::vector<std::vector<Observable>> observables_;

    /** Number of observables. */
    ObservableId number_of_observables_ = 0;

    /*
     * Computed attributes
     */

    /** Sum of the profits of the targets. */
    Profit profit_sum_ = 0;

    friend class InstanceBuilder;

};

}
}
