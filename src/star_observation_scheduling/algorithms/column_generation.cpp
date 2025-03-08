/**
 * Star observation scheduling problem
 *
 * Problem description:
 * See https://github.com/fontanf/orproblems/blob/main/orproblems/starobservationscheduling.hpp
 *
 * The linear programming formulation of the problem based on Dantzig–Wolfe
 * decomposition is written as follows:
 *
 * Variables:
 * - yᵢᵏ ∈ {0, 1} representing a feasible schedule of night i.
 *   yᵢᵏ = 1 iff the corresponding schedule is selected for night i.
 *   xⱼᵢᵏ = 1 iff yᵢᵏ contains target j, otherwise 0.
 *
 * Program:
 *
 * max ∑ᵢ ∑ₖ (∑ⱼ wⱼ xⱼᵢᵏ) yᵢᵏ
 *                                      Note that (∑ⱼ wⱼ xⱼᵢᵏ) is a constant.
 *
 * 0 <= ∑ₖ yᵢᵏ <= 1        for all nights i
 *                         (not more than 1 schedule selected for each night)
 *                                                         Dual variables: uᵢ
 * 0 <= ∑ₖ xⱼᵢᵏ yᵢᵏ <= 1   for all targets j
 *                                        (each target selected at most once)
 *                                                         Dual variables: vⱼ
 *
 * The pricing problem consists in finding a variable of positive reduced cost.
 * The reduced cost of a variable yᵢᵏ is given by:
 * rc(yᵢᵏ) = ∑ⱼ wⱼ xⱼᵢᵏ - uᵢ - ∑ⱼ xⱼᵢᵏ vⱼ
 *         = ∑ⱼ (wⱼ - vⱼ) xⱼᵢᵏ - uᵢ
 *
 * Therefore, finding a variable of maximum reduced cost reduces to solving m
 * Single Night Star Observation Scheduling Problems with targets with profit
 * (wⱼ - vⱼ).
 *
 */

#include "starobservationschedulingsolver/star_observation_scheduling/algorithms/column_generation.hpp"

#include "starobservationschedulingsolver/star_observation_scheduling/algorithm_formatter.hpp"

#include "starobservationschedulingsolver/single_night_star_observation_scheduling/instance_builder.hpp"
#include "starobservationschedulingsolver/single_night_star_observation_scheduling/algorithms/dynamic_programming.hpp"

#include "columngenerationsolver/commons.hpp"
#include "columngenerationsolver/algorithms/greedy.hpp"

using namespace starobservationschedulingsolver::star_observation_scheduling;

namespace
{

using Column = columngenerationsolver::Column;
using Value = columngenerationsolver::Value;
using PricingOutput = columngenerationsolver::PricingSolver::PricingOutput;

class PricingSolver: public columngenerationsolver::PricingSolver
{

public:

    PricingSolver(const Instance& instance):
        instance_(instance),
        fixed_targets_(instance.number_of_targets()),
        fixed_nights_(instance.number_of_nights())
    {  }

    virtual std::vector<std::shared_ptr<const Column>> initialize_pricing(
            const std::vector<std::pair<std::shared_ptr<const Column>, Value>>& fixed_columns);

    virtual PricingOutput solve_pricing(
            const std::vector<Value>& duals);

private:

    const Instance& instance_;

    std::vector<int8_t> fixed_targets_;

    std::vector<int8_t> fixed_nights_;

    std::vector<TargetId> snsosp2sosp_;

};

columngenerationsolver::Model get_model(
        const Instance& instance)
{
    columngenerationsolver::Model model;

    model.objective_sense = optimizationtools::ObjectiveDirection::Maximize;

    // Rows.
    // Not more than 1 schedule selected for each night.
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        columngenerationsolver::Row row;
        row.lower_bound = 0;
        row.upper_bound = 1;
        row.coefficient_lower_bound = 0;
        row.coefficient_upper_bound = 1;
        model.rows.push_back(row);
    }
    // Each target selected at most once.
    for (TargetId target_id = 0;
            target_id < instance.number_of_targets();
            ++target_id) {
        columngenerationsolver::Row row;
        row.lower_bound = 0;
        row.upper_bound = 1;
        row.coefficient_lower_bound = 0;
        row.coefficient_upper_bound = 1;
        model.rows.push_back(row);
    }

    // Pricing solver.
    model.pricing_solver = std::unique_ptr<columngenerationsolver::PricingSolver>(
            new PricingSolver(instance));

    return model;
}

std::vector<std::shared_ptr<const Column>> PricingSolver::initialize_pricing(
            const std::vector<std::pair<std::shared_ptr<const Column>, Value>>& fixed_columns)
{
    std::fill(fixed_targets_.begin(), fixed_targets_.end(), -1);
    std::fill(fixed_nights_.begin(), fixed_nights_.end(), -1);
    for (auto p: fixed_columns) {
        const Column& column = *(p.first);
        Value value = p.second;
        if (value < 0.5)
            continue;
        for (const columngenerationsolver::LinearTerm& element: column.elements) {
            if (element.coefficient < 0.5)
                continue;
            if (element.row < instance_.number_of_nights()) {
                fixed_nights_[element.row] = 1;
            } else {
                fixed_targets_[element.row - instance_.number_of_nights()] = 1;
            }
        }
    }
    return {};
}

struct ColumnExtra
{
    NightId night_id;
    starobservationschedulingsolver::single_night_star_observation_scheduling::Solution snsosp_solution;
    std::vector<ObservableId> snsosp2sosp;
};

PricingOutput PricingSolver::solve_pricing(
            const std::vector<Value>& duals)
{
    PricingOutput output;
    Value reduced_cost_bound = 0.0;

    for (NightId night_id = 0;
            night_id < instance_.number_of_nights();
            ++night_id) {
        if (fixed_nights_[night_id] == 1)
            continue;

        // Build subproblem instance.
        starobservationschedulingsolver::single_night_star_observation_scheduling::InstanceBuilder snsosp_instance_builder;
        snsosp2sosp_.clear();
        for (ObservableId observable_id = 0;
                observable_id < (ObservableId)instance_.night(night_id).observables.size();
                ++observable_id) {
            const Observable& observable = instance_.observable(night_id, observable_id);
            if (fixed_targets_[observable.target_id] == 1)
                continue;
            starobservationschedulingsolver::single_night_star_observation_scheduling::Profit profit
                = (starobservationschedulingsolver::single_night_star_observation_scheduling::Profit)instance_.target(observable.target_id).profit
                - duals[instance_.number_of_nights() + observable.target_id];
            if (profit <= 0)
                continue;
            snsosp_instance_builder.add_target(
                    observable.release_date,
                    observable.meridian,
                    observable.deadline,
                    observable.observation_time,
                    profit);
            snsosp2sosp_.push_back(observable_id);
        }
        starobservationschedulingsolver::single_night_star_observation_scheduling::Instance snsosp_instance = snsosp_instance_builder.build();

        // Solve subproblem instance.
        starobservationschedulingsolver::single_night_star_observation_scheduling::DynamicProgrammingOptionalParameters snsosp_parameters;
        snsosp_parameters.verbosity_level = 0;
        auto snsosp_output = starobservationschedulingsolver::single_night_star_observation_scheduling::dynamic_programming(
                snsosp_instance,
                snsosp_parameters);
        //std::cout << "night_id " << night_id
        //    << " val " << snsosp_output.solution.profit()
        //    << std::endl;

        // Retrieve column.
        Column column;
        columngenerationsolver::LinearTerm element;
        element.row = night_id;
        element.coefficient = 1;
        column.elements.push_back(element);
        for (starobservationschedulingsolver::single_night_star_observation_scheduling::TargetId snsosp_observation_pos = 0;
                snsosp_observation_pos < snsosp_output.solution.number_of_observations();
                ++snsosp_observation_pos) {
            const auto& snsosp_observation = snsosp_output.solution.observation(snsosp_observation_pos);
            ObservableId observable_id = snsosp2sosp_[snsosp_observation.target_id];
            const Observable& observable = instance_.observable(night_id, observable_id);
            columngenerationsolver::LinearTerm element;
            element.row = instance_.number_of_nights() + observable.target_id;
            element.coefficient = 1;
            column.elements.push_back(element);
            column.objective_coefficient += instance_.target(observable.target_id).profit;
        }
        // Extra.
        ColumnExtra extra {night_id, snsosp_output.solution, snsosp2sosp_};
        column.extra = std::shared_ptr<void>(new ColumnExtra(extra));
        output.columns.push_back(std::shared_ptr<const Column>(new Column(column)));

        reduced_cost_bound = (std::max)(
                reduced_cost_bound,
                columngenerationsolver::compute_reduced_cost(column, duals));
    }

    output.overcost = instance_.number_of_nights() * std::max(0.0, reduced_cost_bound);
    return output;
}

Solution columns2solution(
        const Instance& instance,
        const columngenerationsolver::Solution& cg_solution)
{
    std::vector<std::vector<std::pair<ObservableId, Time>>> sol(instance.number_of_nights());
    for (const auto& colval: cg_solution.columns()) {
        std::shared_ptr<ColumnExtra> extra
            = std::static_pointer_cast<ColumnExtra>(colval.first->extra);
        for (starobservationschedulingsolver::single_night_star_observation_scheduling::TargetId snsosp_observation_pos = 0;
                snsosp_observation_pos < extra->snsosp_solution.number_of_observations();
                ++snsosp_observation_pos) {
            const auto& snsosp_observation = extra->snsosp_solution.observation(snsosp_observation_pos);
            ObservableId observable_id = extra->snsosp2sosp[snsosp_observation.target_id];
            sol[extra->night_id].push_back({observable_id, snsosp_observation.start_time});
        }
    }

    Solution solution(instance);
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        sort(sol[night_id].begin(), sol[night_id].end(),
                [](
                    const std::pair<ObservableId, Time>& os1,
                    const std::pair<ObservableId, Time>& os2) -> bool
                {
                    return os1.second < os2.second;
                });
        for (const auto& os: sol[night_id])
            solution.append_observation(night_id, os.first, os.second);
    }

    return solution;
}

}

const ColumnGenerationGreedyOutput starobservationschedulingsolver::star_observation_scheduling::column_generation_greedy(
        const Instance& instance,
        const ColumnGenerationOptionalParameters& parameters)
{
    ColumnGenerationGreedyOutput output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Column generation heuristic - greedy");
    algorithm_formatter.print_header();

    columngenerationsolver::Model model = get_model(instance);
    columngenerationsolver::GreedyParameters greedy_parameters;
    greedy_parameters.timer = parameters.timer;
    greedy_parameters.verbosity_level = 0;
    greedy_parameters.internal_diving = 1;
    greedy_parameters.new_solution_callback = [&instance, &algorithm_formatter](
            const columngenerationsolver::Output& cgs_output)
    {
        if (cgs_output.solution.feasible()) {
            Solution solution = columns2solution(instance, cgs_output.solution);
            algorithm_formatter.update_solution(solution, "");
        }
    };
    greedy_parameters.new_bound_callback = [&instance, &algorithm_formatter](
            const columngenerationsolver::Output& cgs_output)
    {
        Profit bound = std::ceil(cgs_output.bound - FFOT_TOL);
        algorithm_formatter.update_bound(bound, "");
    };
    greedy_parameters.column_generation_parameters.solver_name
        = columngenerationsolver::s2lps(parameters.linear_programming_solver);
    auto greedy_output = columngenerationsolver::greedy(
            model,
            greedy_parameters);

    algorithm_formatter.end();
    return output;
}
