#pragma once

#include "starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/instance.hpp"

#include "optimizationtools/utils/output.hpp"
#include "optimizationtools/utils/utils.hpp"

#include "nlohmann//json.hpp"

#include <string>
#include <iomanip>

namespace starobservationschedulingsolver
{
namespace flexible_single_night_star_observation_scheduling
{

/**
 * Structure for an observation.
 */
struct Observation
{
    /** Target. */
    TargetId target_id;

    /** Observation time. */
    Counter observation_time_pos;

    /** Start time. */
    Time start_time;
};

/**
 * Solution class for a 'flexible_single_night_star_observation_scheduling' problem.
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
            Counter observation_time_pos,
            Time start_time);

    /*
     * Getters
     */

    /** Get the instance. */
    inline const Instance& instance() const { return *instance_; }

    /** Get the number of observations. */
    inline TargetId number_of_observations() const { return observations_.size(); }

    /** Get an observation. */
    inline const Observation& observation(TargetId observation_pos) const { return observations_[observation_pos]; }

    /** Return 'true' iff the solution is feasible. */
    inline bool feasible() const { return true; }

    /** Get the profit of the solution. */
    inline Profit profit() const { return profit_; }

    /** Get the profit of the solution. */
    inline Profit objective_value() const { return profit(); }

    /*
     * Export
     */

    /** Print the instance. */
    std::ostream& format(
            std::ostream& os,
            int verbosity_level = 1) const;

    /** Export solution characteristics to a JSON structure. */
    nlohmann::json to_json() const;

    /** Write the solution to a file. */
    void write(
            const std::string& certificate_path) const;

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

inline optimizationtools::ObjectiveDirection objective_direction()
{
    return optimizationtools::ObjectiveDirection::Maximize;
}

/**
 * Output structure for a 'flexible_single_night_star_observation_scheduling' problem.
 */
struct Output: optimizationtools::Output
{
    /** Constructor. */
    Output(const Instance& instance):
        solution(instance),
        bound(instance.total_profit()) { }


    /** Solution. */
    Solution solution;

    /** Bound. */
    Profit bound = 0;

    /** Elapsed time. */
    double time = 0.0;


    std::string solution_value() const
    {
        return optimizationtools::solution_value(
            objective_direction(),
            solution.feasible(),
            solution.objective_value());
    }

    double absolute_optimality_gap() const
    {
        return optimizationtools::absolute_optimality_gap(
                objective_direction(),
                solution.feasible(),
                solution.objective_value(),
                bound);
    }

    double relative_optimality_gap() const
    {
       return optimizationtools::relative_optimality_gap(
            objective_direction(),
            solution.feasible(),
            solution.objective_value(),
            bound);
    }

    virtual nlohmann::json to_json() const
    {
        return nlohmann::json {
            {"Solution", solution.to_json()},
            {"Value", solution_value()},
            {"Bound", bound},
            {"AbsoluteOptimalityGap", absolute_optimality_gap()},
            {"RelativeOptimalityGap", relative_optimality_gap()},
            {"Time", time}
        };
    }

    virtual int format_width() const { return 30; }

    virtual void format(std::ostream& os) const
    {
        int width = format_width();
        os
            << std::setw(width) << std::left << "Value: " << solution_value() << std::endl
            << std::setw(width) << std::left << "Bound: " << bound << std::endl
            << std::setw(width) << std::left << "Absolute optimality gap: " << absolute_optimality_gap() << std::endl
            << std::setw(width) << std::left << "Relative optimality gap (%): " << relative_optimality_gap() * 100 << std::endl
            << std::setw(width) << std::left << "Time (s): " << time << std::endl
            ;
    }
};

using NewSolutionCallback = std::function<void(const Output&, const std::string&)>;

struct Parameters: optimizationtools::Parameters
{
    /** Callback function called when a new best solution is found. */
    NewSolutionCallback new_solution_callback = [](const Output&, const std::string&) { };


    virtual nlohmann::json to_json() const override
    {
        nlohmann::json json = optimizationtools::Parameters::to_json();
        json.merge_patch({});
        return json;
    }

    virtual int format_width() const override { return 23; }

    virtual void format(std::ostream& os) const override
    {
        optimizationtools::Parameters::format(os);
    }
};

}
}
