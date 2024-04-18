#include "starobservationschedulingsolver/flexible_star_observation_scheduling/utils.hpp"

#include "starobservationschedulingsolver/flexible_star_observation_scheduling/instance_builder.hpp"

using namespace starobservationschedulingsolver::flexible_star_observation_scheduling;

Instance starobservationschedulingsolver::flexible_star_observation_scheduling::sosp2fsosp(
        const starobservationschedulingsolver::star_observation_scheduling::Instance& sosp_instance,
        const std::vector<double>& coefs)
{
    InstanceBuilder instance_builder;
    instance_builder.set_number_of_nights(sosp_instance.number_of_nights());
    instance_builder.set_number_of_targets(sosp_instance.number_of_targets());

    for (NightId night_id = 0;
            night_id < sosp_instance.number_of_nights();
            ++night_id) {
        for (ObservableId sosp_observable_id = 0;
                sosp_observable_id < sosp_instance.number_of_observables(night_id);
                ++sosp_observable_id) {
            const auto& observable = sosp_instance.observable(night_id, sosp_observable_id);
            ObservableId observable_id = instance_builder.add_observable(
                    night_id,
                    observable.target_id,
                    observable.release_date,
                    observable.meridian,
                    observable.deadline);

            // Add shorter observation time.
            for (double coef: coefs) {
                Time reduced_observation_time = std::round(coef * observable.observation_time);
                Profit reduced_profit = sosp_instance.profit(observable.target_id)
                    * reduced_observation_time
                    / observable.observation_time;
                instance_builder.add_observation_time(
                        night_id,
                        observable_id,
                        reduced_observation_time,
                        reduced_profit);
            }

            // Add full duration observation time.
            instance_builder.add_observation_time(
                    night_id,
                    observable_id,
                    observable.observation_time,
                    sosp_instance.profit(observable.target_id));
        }
    }

    return instance_builder.build();
}

Instance starobservationschedulingsolver::flexible_star_observation_scheduling::sosp2fsosp(
        const starobservationschedulingsolver::star_observation_scheduling::Instance& sosp_instance,
        double coef)
{
    InstanceBuilder instance_builder;
    instance_builder.set_number_of_nights(sosp_instance.number_of_nights());
    instance_builder.set_number_of_targets(sosp_instance.number_of_targets());

    for (NightId night_id = 0;
            night_id < sosp_instance.number_of_nights();
            ++night_id) {
        for (ObservableId sosp_observable_id = 0;
                sosp_observable_id < sosp_instance.number_of_observables(night_id);
                ++sosp_observable_id) {
            const auto& observable = sosp_instance.observable(night_id, sosp_observable_id);
            ObservableId observable_id = instance_builder.add_observable(
                    night_id,
                    observable.target_id,
                    observable.release_date,
                    observable.meridian,
                    observable.deadline);

            // Add shorter observation time.
            Time observation_time_min = std::round(coef * observable.observation_time);
            for (Time observation_time = observation_time_min;
                    observation_time <= observable.observation_time;
                    ++observation_time) {
                Profit profit = sosp_instance.profit(observable.target_id)
                    * observation_time
                    / observable.observation_time;
                instance_builder.add_observation_time(
                        night_id,
                        observable_id,
                        observation_time,
                        profit);
            }
        }
    }

    return instance_builder.build();
}
