#pragma once

#include "starobservationschedulingsolver/singlenightstarobservationscheduling/instance.hpp"

#include <string>

namespace starobservationschedulingsolver
{
namespace singlenightstarobservationscheduling
{

/**
 * Structure for an observation.
 */
struct Observation
{
    /** Target. */
    TargetId target_id;

    /** Start time. */
    Time start_time;
};

/**
 * Solution class for a 'singlenightstarobservationschedulingsolver' problem.
 */
class Solution
{

public:

    /*
     * Constructors and destructor
     */

    /** Constructor. */
    Solution(const Instance& instance);

    /** Add an observation. */
    void append_observation(
            TargetId target_id,
            Time start_time);

    /** Get the instance. */
    const Instance& instance() const { return *instance_; }

    /** Get the number of observations. */
    TargetId number_of_observations() const { return observations_.size(); }

    /** Get an observation. */
    const Observation& observation(TargetId observation_pos) const { return observations_[observation_pos]; }

    /** Return 'true' iff the solution is feasible. */
    bool feasible() const { return true; }

    /** Get the profit of the solution. */
    Profit profit() const { return profit_; }

    /*
     * Export.
     */

    /** Print the instance. */
    std::ostream& print(
            std::ostream& os,
            int verbose = 1) const;

    /** Write the solution to a file. */
    void write(std::string certificate_path) const;

private:

    /*
     * Private attributes
     */

    /** Instance. */
    const Instance* instance_;

    /** Targets. */
    std::vector<TargetId> targets_;

    /** Observations. */
    std::vector<Observation> observations_;

    /** Current time. */
    Time current_time_ = 0;

    /** Profit of the solution. */
    Profit profit_ = 0;

};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Output ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Output structure for a 'singlenightstarobservationscheduling' problem.
 */
struct Output
{
    /** Constructor. */
    Output(
            const Instance& instance,
            optimizationtools::Info& info);

    /** Solution. */
    Solution solution;

    /** Bound. */
    Profit bound = std::numeric_limits<Profit>::infinity();

    /** Elapsed time. */
    double time = -1;

    /** Print current state. */
    void print(
            optimizationtools::Info& info,
            const std::stringstream& s) const;

    /** Update the solution. */
    void update_solution(
            const Solution& solution_new,
            const std::stringstream& s,
            optimizationtools::Info& info);

    /** Update the bound. */
    void update_bound(
            Profit bound_new,
            const std::stringstream& s,
            optimizationtools::Info& info);

    /** Print the algorithm statistics. */
    virtual void print_statistics(
            optimizationtools::Info& info) const { (void)info; }

    /** Method to call at the end of the algorithm. */
    Output& algorithm_end(optimizationtools::Info& info);
};

}
}
