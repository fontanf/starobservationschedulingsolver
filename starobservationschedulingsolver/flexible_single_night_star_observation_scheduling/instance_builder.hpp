#pragma once

#include "starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace flexible_single_night_star_observation_scheduling
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder() { }

    /** Add a target. */
    TargetId add_target(
            Time release_date,
            Time meridian,
            Time deadline);

    /** Add an observation time for a target. */
    void add_observation_time(
            TargetId target_id,
            Time observation_time,
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

    /** Read an instance from a file in 'default' format. */
    void read_default(std::ifstream& file);

    /*
     * Private attributes
     */

    /** Instance. */
    Instance instance_;

};

}
}
