/**
 * Star observation scheduling problem
 *
 * Input:
 * - m nights
 * - n targets; for each target j = 1..n, a profit wⱼ
 * - a list of possible observations. An observation is associated to a night i
 *   and a target j and has a time-window [rᵢⱼ, dᵢⱼ] and a duration pᵢⱼ such
 *   that 2 pⱼᵢ ≥ dⱼᵢ - rⱼᵢ
 * Problem:
 * - select a list of observations and their starting dates sᵢⱼ such that:
 *   - a target is observed at most once
 *   - observations do not overlap
 *   - starting dates satisfy the time-windows, i.e. rᵢⱼ <= sᵢⱼ and
 *     sᵢⱼ + pᵢⱼ <= dᵢⱼ
 * Objective:
 * - maximize the overall profit of the selected observations
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <limits>

namespace starobservationschedulingsolver
{
namespace star_observation_scheduling
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
 * Structure for a target.
 */
struct Target
{
    /** Profit of the target. */
    Profit profit = 0;

    /** Observables of the target. */
    std::vector<std::pair<NightId, ObservableId>> observables;
};

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

    /** Observation time of the observable. */
    Time observation_time;
};

struct Night
{
    /** Observables of the night. */
    std::vector<Observable> observables;

    /** Start time. */
    Time start = std::numeric_limits<Time>::max();

    /** End time. */
    Time end = std::numeric_limits<Time>::min();
};

/**
 * Instance class for a 'starobservationscheduling' problem.
 */
class Instance
{

public:

    /*
     * Getters
     */

    /** Get the number of nights. */
    NightId number_of_nights() const { return nights_.size(); }

    /** Get a night. */
    const Night& night(NightId night_id) const { return nights_[night_id]; }

    /** Get the number of targets. */
    TargetId number_of_targets() const { return targets_.size(); }

    /** Get a target. */
    const Target& target(TargetId target_id) const { return targets_[target_id]; }

    /** Get the total number of observables. */
    ObservableId number_of_observables() const { return number_of_observables_; }

    /** Get an observble. */
    const Observable& observable(
            NightId night_id,
            ObservableId observable_id) const
    {
        return nights_[night_id].observables[observable_id];
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

    /** Nights. */
    std::vector<Night> nights_;

    /** Number of observables. */
    ObservableId number_of_observables_ = 0;

    /** Targets. */
    std::vector<Target> targets_;

    /*
     * Computed attributes
     */

    /** Sum of the profits of the targets. */
    Profit profit_sum_ = 0;

    friend class InstanceBuilder;

};

}
}
