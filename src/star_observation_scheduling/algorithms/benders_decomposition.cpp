#ifdef XPRESS_FOUND

#include "starobservationschedulingsolver/star_observation_scheduling/algorithms/benders_decomposition.hpp"

#include "starobservationschedulingsolver/star_observation_scheduling/algorithm_formatter.hpp"

#include <xprs.h>

using namespace starobservationschedulingsolver::star_observation_scheduling;

namespace
{

struct XpressRow
{
    std::vector<int> indices;
    std::vector<double> coefficients;
    double rhs;
    double rng;
    char type;
};

std::vector<ObservableId> remove_element(
        const std::vector<ObservableId>& observable_ids,
        ObservableId pos)
{
    std::vector<ObservableId> observable_ids_new;
    for (ObservableId p = 0; p < (ObservableId)observable_ids.size(); ++p) {
        if (p == pos)
            continue;
        observable_ids_new.push_back(observable_ids[p]);
    }
    return observable_ids_new;
}

bool check(
        const Instance& instance,
        NightId night_id,
        const std::vector<ObservableId>& observable_ids,
        ObservableId pos = -1)
{
    Time current_time = std::numeric_limits<Time>::min();
    for (ObservableId p = 0; p < (ObservableId)observable_ids.size(); ++p) {
        if (p == pos)
            continue;
        const Observable& observable = instance.observable(night_id, observable_ids[p]);
        current_time = std::max(current_time, observable.release_date);
        if (current_time + observable.observation_time
                > observable.deadline) {
            return false;
        }
        current_time += observable.observation_time;
    }
    return true;
}

}

const BendersDecompositionGreedyOutput starobservationschedulingsolver::star_observation_scheduling::benders_decomposition(
        const Instance& instance,
        const BendersDecompositionOptionalParameters& parameters)
{
    BendersDecompositionGreedyOutput output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Benders decomposition");
    algorithm_formatter.print_header();

    // Initialize Xpress.
    auto rc = XPRSinit(NULL);
    if (rc) {
        std::cout << "Problem with XPRSinit\n" << std::endl;
        algorithm_formatter.end();
        return output;
    }

    // Build MILP model.
    XPRSprob xpress_problem;
    XPRScreateprob(&xpress_problem);
    // Set objective sense.
    XPRSchgobjsense(xpress_problem, XPRS_OBJ_MAXIMIZE);

    // Variables.
    // x[i][j] = 1 iff observation j of night i is performed, 0 otherwise.
    std::vector<std::pair<NightId, ObservableId>> xpress2sosp;
    std::vector<std::vector<int>> sosp2xpress(instance.number_of_nights());
    int xpress_number_of_variables = 0;
    int start[] = {0};
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        sosp2xpress[night_id] = std::vector<int>(instance.number_of_observables(night_id));
        for (ObservableId observable_id = 0;
                observable_id < instance.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance.observable(night_id, observable_id);
            std::string xpress_variable_name
                = "x" + std::to_string(night_id)
                + "," + std::to_string(observable_id);
            double objective_coefficient = instance.profit(observable.target_id);
            double lower_bound = 0;
            double upper_bound = 1;
            XPRSaddcols(
                    xpress_problem,
                    1,
                    0,
                    &objective_coefficient,
                    start,
                    NULL,
                    NULL,
                    &lower_bound,
                    &upper_bound);
            char type = 'B';
            XPRSchgcoltype(
                    xpress_problem,
                    1,
                    &xpress_number_of_variables,
                    &type);
            xpress2sosp.push_back({night_id, observable_id});
            sosp2xpress[night_id][observable_id] = xpress_number_of_variables;
            xpress_number_of_variables++;
        }
    }

    // Constraints: each target is observed at most once.
    std::vector<XpressRow> xpress_constraints_demand(instance.number_of_targets());
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        for (ObservableId observable_id = 0;
                observable_id < instance.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance.observable(night_id, observable_id);
            xpress_constraints_demand[observable.target_id].indices.push_back(
                    sosp2xpress[night_id][observable_id]);
            xpress_constraints_demand[observable.target_id].coefficients.push_back(1);
        }
    }
    for (TargetId target_id = 0;
            target_id < instance.number_of_targets();
            ++target_id) {
        xpress_constraints_demand[target_id].type = 'L';
        xpress_constraints_demand[target_id].rhs = 1;
        XPRSaddrows(
                xpress_problem,
                1,
                xpress_constraints_demand[target_id].indices.size(),
                &xpress_constraints_demand[target_id].type,
                &xpress_constraints_demand[target_id].rhs,
                &xpress_constraints_demand[target_id].rng,
                start,
                xpress_constraints_demand[target_id].indices.data(),
                xpress_constraints_demand[target_id].coefficients.data());
        std::string xpress_constraint_name = "demand_" + std::to_string(target_id);
    }

    // Constraints: for each night, a maximum allocated time.
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        XpressRow constraint;
        Time release_date_min = std::numeric_limits<Time>::max();
        Time deadline_max = std::numeric_limits<Time>::min();
        for (ObservableId observable_id = 0;
                observable_id < instance.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance.observable(night_id, observable_id);
            release_date_min = std::min(
                    release_date_min,
                    observable.release_date);
            deadline_max = std::max(
                    deadline_max,
                    observable.deadline);
        }
        for (ObservableId observable_id = 0;
                observable_id < instance.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance.observable(night_id, observable_id);
            constraint.indices.push_back(sosp2xpress[night_id][observable_id]);
            constraint.coefficients.push_back(observable.observation_time);
        }
        constraint.type = 'L';
        constraint.rhs = std::max((Time)0, deadline_max - release_date_min);
        XPRSaddrows(
                xpress_problem,
                1,
                constraint.indices.size(),
                &constraint.type,
                &constraint.rhs,
                &constraint.rng,
                start,
                constraint.indices.data(),
                constraint.coefficients.data());
        std::string xpress_constraint_name = "time_" + std::to_string(night_id);
    }

    // Constraints: pairs of incompatible observations.
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        for (ObservableId observable_id_1 = 0;
                observable_id_1 < instance.number_of_observables(night_id);
                ++observable_id_1) {
            const Observable& observable_1 = instance.observable(night_id, observable_id_1);
            Time mandatory_start_1 = observable_1.deadline - observable_1.observation_time;
            Time mandatory_end_1 = observable_1.release_date + observable_1.observation_time;
            for (ObservableId observable_id_2 = observable_id_1 + 1;
                    observable_id_2 < instance.number_of_observables(night_id);
                    ++observable_id_2) {
                const Observable& observable_2 = instance.observable(night_id, observable_id_2);
                Time mandatory_start_2 = observable_2.deadline - observable_2.observation_time;
                Time mandatory_end_2 = observable_2.release_date + observable_2.observation_time;
                if (mandatory_start_1 >= mandatory_end_2)
                    continue;
                if (mandatory_end_1 <= mandatory_start_2)
                    continue;
                XpressRow constraint;
                constraint.indices.push_back(sosp2xpress[night_id][observable_id_1]);
                constraint.indices.push_back(sosp2xpress[night_id][observable_id_2]);
                constraint.coefficients.push_back(1);
                constraint.coefficients.push_back(1);
                constraint.type = 'L';
                constraint.rhs = 1;
                XPRSaddrows(
                        xpress_problem,
                        1,
                        constraint.indices.size(),
                        &constraint.type,
                        &constraint.rhs,
                        &constraint.rng,
                        start,
                        constraint.indices.data(),
                        constraint.coefficients.data());
                //std::cout << "night_id " << night_id
                //    << " observable_id_1 " << observable_id_1
                //    << " observable_id_2 " << observable_id_2
                //    << " target_id_1 " << observable_1.target_id
                //    << " target_id_2 " << observable_2.target_id
                //    << std::endl;
                std::string xpress_constraint_name = "conflict_"
                    + std::to_string(night_id)
                    + "_" + std::to_string(observable_id_1)
                    + "_" + std::to_string(observable_id_2);
            }
        }
    }

    // Benders loop.
    for (output.number_of_iterations = 0;;
            ++output.number_of_iterations) {

        std::string xpress_log_path = "xpress_" + std::to_string(output.number_of_iterations) + ".log";
        XPRSsetlogfile(
                xpress_problem,
                xpress_log_path.c_str());

        // Solve MILP model.
        XPRSmipoptimize(xpress_problem, "");

        // Update bound.
        double xpress_bound_value = 0.0;
        XPRSgetdblattrib(xpress_problem, XPRS_BESTBOUND, &xpress_bound_value);
        std::stringstream ss;
        ss << "iteration " << output.number_of_iterations;
        algorithm_formatter.update_bound(xpress_bound_value, ss.str());

        // Retrieve MILP solution.
        bool ok = true;
        Solution solution(instance);
        std::vector<double> xpress_solution(xpress_number_of_variables, 0.0);
        XPRSgetmipsol(xpress_problem, xpress_solution.data(), NULL);
        for (NightId night_id = 0;
                night_id < instance.number_of_nights();
                ++night_id) {
            // For each night, check if the assignment is feasible.
            std::vector<ObservableId> observable_ids;
            for (ObservableId observable_id = 0;
                    observable_id < instance.number_of_observables(night_id);
                    ++observable_id) {
                double val = std::round(xpress_solution[sosp2xpress[night_id][observable_id]]);
                if (val >= 0.5)
                    observable_ids.push_back(observable_id);
            }
            bool current_night_ok = true;
            sort(observable_ids.begin(), observable_ids.end(),
                    [&instance, night_id](
                        ObservableId observable_id_1,
                        ObservableId observable_id_2) -> bool
                    {
                        const Observable& observable_1 = instance.observable(night_id, observable_id_1);
                        const Observable& observable_2 = instance.observable(night_id, observable_id_2);
                        return observable_1.meridian < observable_2.meridian;
                    });
            current_night_ok = check(instance, night_id, observable_ids);

            if (current_night_ok) {

                Time current_time = std::numeric_limits<Time>::min();
                for (ObservableId observable_id: observable_ids) {
                    const Observable& observable = instance.observable(night_id, observable_id);
                    current_time = std::max(current_time, observable.release_date);
                    solution.append_observation(night_id, observable_id, current_time);
                    current_time += observable.observation_time;
                }

            } else {

                // If not, add cuts.
                ok = false;

                //std::cout << "Schedule of night " << night_id << " is not feasible." << std::endl;
                //for (ObservableId observable_id: observable_ids)
                //    std::cout << " " << observable_id;
                //std::cout << std::endl;

                std::vector<std::vector<ObservableId>> stack{observable_ids};
                while (!stack.empty()) {
                    // Retrieve subset of observables.
                    auto obs_ids = stack.back();
                    stack.pop_back();

                    // Check if all subset obtained by removing one element are
                    // feasible.
                    bool minimal = true;
                    for (ObservableId pos = 0; pos < (ObservableId)obs_ids.size(); ++pos) {
                        bool feasible = check(instance, night_id, obs_ids, pos);
                        if (!feasible) {
                            minimal = false;
                            break;
                        }
                    }

                    if (minimal) {
                        // If yes, this subset is minimal. We add the cut.
                        XpressRow constraint;
                        for (ObservableId observable_id: obs_ids) {
                            constraint.indices.push_back(sosp2xpress[night_id][observable_id]);
                            constraint.coefficients.push_back(1);
                        }
                        constraint.type = 'L';
                        constraint.rhs = obs_ids.size() - 1;
                        XPRSaddrows(
                                xpress_problem,
                                1,
                                constraint.indices.size(),
                                &constraint.type,
                                &constraint.rhs,
                                &constraint.rng,
                                start,
                                constraint.indices.data(),
                                constraint.coefficients.data());
                        std::string xpress_constraint_name = "cut_"
                            + std::to_string(output.number_of_iterations)
                            + "_" + std::to_string(night_id);

                        //std::cout << "add constraint" << std::endl;
                        //for (ObservableId observable_id: obs_ids)
                        //    std::cout << " " << observable_id;
                        //std::cout << std::endl;
                    } else {
                        // Otherwise, add these new subsets to the stack.
                        for (ObservableId pos = 0;
                                pos < (ObservableId)obs_ids.size();
                                ++pos) {
                            stack.push_back(remove_element(obs_ids, pos));
                        }
                    }
                }
            }
        }

        { // Update best solution.
            std::stringstream ss;
            ss << "iteration " << output.number_of_iterations;
            algorithm_formatter.update_solution(solution, ss.str());
        }
        if (output.bound == output.solution.profit())
            break;

        if (ok)
            break;
    }

    // Free Xpress.
    XPRSdestroyprob(xpress_problem);
    XPRSfree();

    algorithm_formatter.end();
    return output;
}

#endif
