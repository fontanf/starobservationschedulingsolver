#pragma once

#include "optimizationtools/utils/info.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblesinglenightstarobservationscheduling
{

using TargetId = int64_t;
using Profit = double;
using Time = int64_t;
using Counter = int64_t;

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

    /** Observation times. */
    std::vector<Time> observation_times;

    /** Profits. */
    std::vector<Profit> profits;

    /** Maximum profit. */
    Profit maximum_profit = 0;
};

/**
 * Instance class for a 'flexiblesinglenightstarobservationschedulingsolver' problem.
 */
class Instance
{

public:

    /*
     * Constructors and destructor
     */

    /** Build an instance from a file. */
    Instance(
            std::string instance_path,
            std::string format = "");

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
    std::ostream& print(
            std::ostream& os,
            int verbose = 1) const;

private:

    /*
     * Private methods
     */

    /** Create an instance manually. */
    Instance() { }

    /** Read an instance from a file in 'default' format. */
    void read_default(std::ifstream& file);

    /*
     * Private attributes
     */

    /** Targets. */
    std::vector<Target> targets_;

    /** Total profit. */
    Profit total_profit_ = 0;

    friend class InstanceBuilder;

};

void init_display(
        const Instance& instance,
        optimizationtools::Info& info);

}
}
