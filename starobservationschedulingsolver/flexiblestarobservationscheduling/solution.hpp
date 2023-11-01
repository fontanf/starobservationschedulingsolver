#pragma once

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/instance.hpp"

#include <string>

namespace starobservationschedulingsolver
{
namespace flexiblestarobservationscheduling
{

/**
 * Structure for an observation.
 */
struct Observation
{
    /** Observable. */
    ObservableId observable_id;

    /** Observation time. */
    Counter observation_time_pos;

    /** Start time. */
    Time start_time;
};

struct SolutionNight
{
    /** Observations. */
    std::vector<Observation> observations;

    /** Current time. */
    Time current_time = 0;
};

/**
 * Solution class for a 'flexiblestarobservationschedulingsolver' problem.
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
            NightId night_id,
            ObservableId observable_id,
            Counter observation_time_pos,
            Time start_time);

    /** Get the instance. */
    const Instance& instance() const { return *instance_; }

    /** Get the number of observations. */
    TargetId number_of_observations() const { return number_of_observations_; }

    /** Get a night. */
    const SolutionNight& night(NightId night_id) const { return nights_[night_id]; }

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
    std::vector<SolutionNight> nights_;

    /** Number of observations. */
    ObservableId number_of_observations_ = 0;

    /** Profit of the solution. */
    Profit profit_ = 0;

};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Output ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Output structure for a 'flexiblestarobservationscheduling' problem.
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
