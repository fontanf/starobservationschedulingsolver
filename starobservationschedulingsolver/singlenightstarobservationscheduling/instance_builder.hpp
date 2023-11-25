#pragma once

#include "starobservationschedulingsolver/singlenightstarobservationscheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace singlenightstarobservationscheduling
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder() { }

    /** Add a target. */
    void add_target(
            Time release_date,
            Time meridian,
            Time deadline,
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
