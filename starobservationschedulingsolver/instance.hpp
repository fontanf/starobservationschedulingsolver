/**
 * Star Observation Scheduling Problem.
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

#include "optimizationtools/utils/info.hpp"

namespace starobservationschedulingsolver
{

using NightId = int64_t;
using TargetId = int64_t;
using ObservableId = int64_t;
using ObservablePos = int64_t;
using Profit = int64_t;
using Time = int64_t;
using Counter = int64_t;

/**
 * Structure for an observable.
 */
struct Observable
{
    /** Target of the observable. */
    TargetId target_id;

    /** Release date of the observable. */
    Time release_date;

    /** Deadline of the observable. */
    Time deadline;

    /** Observation time of the observable. */
    Time observation_time;
};

/**
 * Instance class for a 'starobservationscheduling' problem.
 */
class Instance
{

public:

    /*
     * Constructors and destructor
     */

    /** Constructor to build an instance manually. */
    Instance(
            NightId number_of_nights,
            TargetId number_of_targets):
        observables_(number_of_nights),
        profits_(number_of_targets) {  }

    /** Add an observable. */
    void add_observable(
            NightId night_id,
            TargetId target_id,
            Time release_date,
            Time deadline,
            Time observation_time);

    /** Set the profit of a target. */
    void set_profit(
            TargetId target_id,
            Profit profit);

    /** Build an instance from a file. */
    Instance(
            std::string instance_path,
            std::string format = "");

    /*
     * Getters
     */

    /** Get the number of nights. */
    NightId number_of_nights() const { return observables_.size(); }

    /** Get the number of targets. */
    TargetId number_of_targets() const { return profits_.size(); }

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

    /** Get the profit of a target. */
    Profit profit(TargetId target_id) const { return profits_[target_id]; }

    /** Get the total profit of the targets. */
    Profit total_profit() const { return profit_sum_; }

    /*
     * Export
     */

    /** Print the instance. */
    std::ostream& print(
            std::ostream& os,
            int verbose = 1) const;

    /*
     * Checkers
     */

    /** Check a certificate. */
    std::pair<bool, Profit> check(
            std::string certificate_path,
            std::ostream& os,
            int verbose = 1) const;

private:

    /*
     * Private methods
     */

    /** Read an instance from a file in 'catusse2016' format. */
    void read_catusse2016(std::ifstream& file);

    /*
     * Private attributes
     */

    /** Observables. */
    std::vector<std::vector<Observable>> observables_;

    /** Number of observables. */
    ObservableId number_of_observables_ = 0;

    /** Profits of the targets. */
    std::vector<Profit> profits_;

    /*
     * Computed attributes
     */

    /** Sum of the profits of the targets. */
    Profit profit_sum_ = 0;

};

void init_display(
        const Instance& instance,
        optimizationtools::Info& info);

}
