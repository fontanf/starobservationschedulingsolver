/**
 * Single-night star observation scheduling problem
 *
 * Input:
 * - n targets with profit wⱼ, time-window [rⱼ, dⱼ] and duration pⱼ such
 *   that 2 pⱼ ≥ dⱼ - rⱼ (j = 1..n)
 * Problem:
 * - select a list of targets and their starting dates sⱼ such that:
 *   - a target is observed at most once
 *   - observations do not overlap
 *   - starting dates satisfy the time-windows, i.e. rⱼ <= sⱼ and
 *     sⱼ + pⱼ <= dⱼ
 * Objective:
 * - maximize the overall profit of the selected targets
 */

#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

namespace starobservationschedulingsolver
{
namespace single_night_star_observation_scheduling
{

using TargetId = int64_t;
using Profit = double;
using Time = int64_t;
using Counter = int64_t;
using Seed = int64_t;

/**
 * Structure for a target.
 */
struct Target
{
    /** Release date. */
    Time release_date;

    /** Meridian of the observable. */
    Time meridian;

    /** Deadline. */
    Time deadline;

    /** Observation time. */
    Time observation_time;

    /** Profit; */
    Profit profit;
};

/**
 * Instance class for a 'single_night_star_observation_scheduling' problem.
 */
class Instance
{

public:

    /*
     * Getters
     */

    /** Get the number of targets. */
    TargetId number_of_targets() const { return targets_.size(); }

    /** Get a target. */
    const Target& target(TargetId target_id) const { return targets_[target_id]; }

    /** Get the total profit of the targets. */
    Profit total_profit() const { return total_profit_; }

    /*
     * Export
     */

    /** Print the instance. */
    std::ostream& format(
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

    /** Targets. */
    std::vector<Target> targets_;

    /** Total profit. */
    Profit total_profit_ = 0;

    friend class InstanceBuilder;

};

}
}
