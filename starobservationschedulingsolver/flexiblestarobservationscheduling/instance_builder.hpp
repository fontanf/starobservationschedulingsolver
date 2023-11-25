#pragma once

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/instance.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblestarobservationscheduling
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder(
            NightId number_of_nights,
            TargetId number_of_targets);

    /** Add an observable. */
    ObservableId add_observable(
            NightId night_id,
            TargetId target_id,
            Time release_date,
            Time meridian,
            Time deadline);

    /** Add an observation time for an observable. */
    void add_observation_time(
            NightId night_id,
            ObservableId observable_id,
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
