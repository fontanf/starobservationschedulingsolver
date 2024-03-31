#pragma once

#include "starobservationschedulingsolver/single_night_star_observation_scheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace single_night_star_observation_scheduling
{

class AlgorithmFormatter
{

public:

    /** Constructor. */
    AlgorithmFormatter(
            const Parameters& parameters,
            Output& output):
        parameters_(parameters),
        output_(output),
        os_(parameters.create_os()) { }

    /** Print the header. */
    void start(
            const std::string& algorithm_name);

    /** Print the header. */
    void print_header();

    /** Print current state. */
    void print(
            const std::string& s);

    /** Update the solution. */
    void update_solution(
            const Solution& solution,
            const std::string& s);

    /** Update the bound. */
    void update_bound(
            Profit bound,
            const std::string& s);

    /** Method to call at the end of the algorithm. */
    void end();

private:

    /** Parameters. */
    const Parameters& parameters_;

    /** Output. */
    Output& output_;

    /** Output stream. */
    std::unique_ptr<optimizationtools::ComposeStream> os_;

};

}
}
