#pragma once

#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblesinglenightstarobservationscheduling
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


    /*
     * Build
     */

    /** Build. */
    Instance build();

private:

    /*
     * Private attributes
     */

    /** Instance. */
    Instance instance_;

};

}
}
