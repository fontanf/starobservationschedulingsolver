#pragma once

#include "starobservationschedulingsolver/starobservationscheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace starobservationscheduling
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder(
            NightId number_of_nights,
            TargetId number_of_targets);

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
