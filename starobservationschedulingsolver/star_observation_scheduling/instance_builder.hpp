#pragma once

#include "starobservationschedulingsolver/star_observation_scheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace star_observation_scheduling
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder() { }

    /** Set the number of nights. */
    void set_number_of_nights(NightId number_of_nights);

    /** Set the number of targets. */
    void set_number_of_targets(TargetId number_of_targets);

    /** Add an observable. */
    void add_observable(
            NightId night_id,
            TargetId target_id,
            Time release_date,
            Time meridian,
            Time deadline,
            Time observation_time);

    /** Set the profit of a target. */
    void set_profit(
            TargetId target_id,
            Profit profit);

    /** Read an instance from a file. */
    void read(
            const std::string& instance_path,
            const std::string& format);

    /*
     * Build
     */

    /** Build. */
    Instance build();

private:

    /*
     * Private methods
     */

    /*
     * Read input file
     */

    /** Read an instance file in 'catusse2016' format. */
    void read_catusse2016(std::ifstream& file);

    /*
     * Private attributes
     */

    /** Instance. */
    Instance instance_;

};

}
}
