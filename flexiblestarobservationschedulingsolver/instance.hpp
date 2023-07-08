#pragma once

#include "optimizationtools/utils/info.hpp"

namespace flexiblestarobservationschedulingsolver
{

using NightId = int64_t;
using TargetId = int64_t;
using ObservableId = int64_t;
using ObservablePos = int64_t;
using Profit = double;
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
 * Instance class for a 'flexiblestarobservationscheduling' problem.
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
        number_of_targets_(number_of_targets),
        observables_(number_of_nights) {  }

    /** Add an observable. */
    ObservableId add_observable(
            NightId night_id,
            TargetId target_id,
            Time release_date,
            Time meridian,
            Time deadline);

    /** Add an observation time for an observable. */
    void add_observation_time(
            NightId night_id,
            ObservableId observable_id,
            Time observation_time,
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
    std::ostream& print(
            std::ostream& os,
            int verbose = 1) const;

    /** Write the instance to a file. */
    void write(std::string instance_path) const;

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
    void read_catusse2016(
            std::ifstream& file);

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

};

void init_display(
        const Instance& instance,
        optimizationtools::Info& info);

}
