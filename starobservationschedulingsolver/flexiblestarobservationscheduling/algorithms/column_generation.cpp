#include "columngenerationsolver/commons.hpp"

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/algorithms/column_generation.hpp"

#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/algorithms/dynamic_programming.hpp"

#include "columngenerationsolver/algorithms/greedy.hpp"

using namespace starobservationschedulingsolver::flexiblestarobservationscheduling;

namespace
{

typedef columngenerationsolver::RowIdx RowIdx;
typedef columngenerationsolver::ColIdx ColIdx;
typedef columngenerationsolver::Value Value;
typedef columngenerationsolver::Column Column;

class PricingSolver: public columngenerationsolver::PricingSolver
{

public:

    PricingSolver(const Instance& instance):
        instance_(instance),
        fixed_targets_(instance.number_of_targets()),
        fixed_nights_(instance.number_of_nights())
    {  }

    virtual std::vector<ColIdx> initialize_pricing(
            const std::vector<Column>& columns,
            const std::vector<std::pair<ColIdx, Value>>& fixed_columns);

    virtual std::vector<Column> solve_pricing(
            const std::vector<Value>& duals);

private:

    const Instance& instance_;

    std::vector<int8_t> fixed_targets_;
    std::vector<int8_t> fixed_nights_;

    std::vector<std::pair<TargetId, std::vector<Counter>>> snsosp2sosp_;

};

columngenerationsolver::Parameters get_parameters(const Instance& instance)
{
    NightId m = instance.number_of_nights();
    TargetId n = instance.number_of_targets();
    columngenerationsolver::Parameters p(m + n);

    p.objective_sense = columngenerationsolver::ObjectiveSense::Max;
    p.column_lower_bound = 0;
    p.column_upper_bound = 1;
    // Row lower bounds.
    std::fill(p.row_lower_bounds.begin(), p.row_lower_bounds.begin() + m, 0);
    std::fill(p.row_lower_bounds.begin() + m, p.row_lower_bounds.end(), 0);
    // Row upper bounds.
    std::fill(p.row_upper_bounds.begin(), p.row_upper_bounds.begin() + m, 1);
    std::fill(p.row_upper_bounds.begin() + m, p.row_upper_bounds.end(), 1);
    // Row coefficent lower bounds.
    std::fill(p.row_coefficient_lower_bounds.begin(), p.row_coefficient_lower_bounds.end(), 0);
    // Row coefficent upper bounds.
    std::fill(p.row_coefficient_upper_bounds.begin(), p.row_coefficient_upper_bounds.end(), 1);
    // Dummy column objective coefficient.
    p.dummy_column_objective_coefficient = -instance.total_profit();
    // Pricing solver.
    p.pricing_solver = std::unique_ptr<columngenerationsolver::PricingSolver>(
            new PricingSolver(instance));
    return p;
}

std::vector<ColIdx> PricingSolver::initialize_pricing(
            const std::vector<Column>& columns,
            const std::vector<std::pair<ColIdx, Value>>& fixed_columns)
{
    std::fill(fixed_targets_.begin(), fixed_targets_.end(), -1);
    std::fill(fixed_nights_.begin(), fixed_nights_.end(), -1);
    for (auto p: fixed_columns) {
        const Column& column = columns[p.first];
        Value value = p.second;
        if (value < 0.5)
            continue;
        for (RowIdx row_pos = 0; row_pos < (RowIdx)column.row_indices.size(); ++row_pos) {
            RowIdx row_index = column.row_indices[row_pos];
            Value row_coefficient = column.row_coefficients[row_pos];
            if (row_coefficient < 0.5)
                continue;
            if (row_index < instance_.number_of_nights()) {
                fixed_nights_[row_index] = 1;
            } else {
                fixed_targets_[row_index - instance_.number_of_nights()] = 1;
            }
        }
    }
    return {};
}

struct ColumnExtra
{
    NightId night_id;
    starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::Solution snsosp_solution;
    std::vector<std::pair<ObservableId, std::vector<Counter>>> snsosp2sosp;
};

std::vector<Column> PricingSolver::solve_pricing(
            const std::vector<Value>& duals)
{
    std::vector<Column> columns;
    for (NightId night_id = 0;
            night_id < instance_.number_of_nights();
            ++night_id) {
        if (fixed_nights_[night_id] == 1)
            continue;

        // Build subproblem instance.
        starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::Instance snsosp_instance;
        snsosp2sosp_.clear();
        for (ObservableId observable_id = 0;
                observable_id < instance_.number_of_observables(night_id);
                ++observable_id) {
            const Observable& observable = instance_.observable(night_id, observable_id);
            if (fixed_targets_[observable.target_id] == 1)
                continue;
            starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::TargetId snsosp_target_id = -1;
            for (Counter observation_time_pos = 0;
                    observation_time_pos < (Counter)observable.observation_times.size();
                    ++observation_time_pos) {
                starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::Profit profit
                    = observable.profits[observation_time_pos]
                    - duals[instance_.number_of_nights() + observable.target_id];
                if (profit <= 0)
                    continue;
                if (snsosp_target_id == -1) {
                    snsosp_target_id = snsosp_instance.add_target(
                            observable.release_date,
                            observable.meridian,
                            observable.deadline);
                    snsosp2sosp_.push_back({observable_id, std::vector<Counter>()});
                }
                snsosp_instance.add_observation_time(
                        snsosp_target_id,
                        observable.observation_times[observation_time_pos],
                        profit);
                snsosp2sosp_[snsosp_target_id].second.push_back(observation_time_pos);
            }
        }

        // Solve subproblem instance.
        starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::DynamicProgrammingOptionalParameters snsosp_parameters;
        //snsosp_parameters.info.set_verbosity_level(1);
        auto snsosp_output = starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::dynamic_programming(
                snsosp_instance,
                snsosp_parameters);
        //std::cout << "night_id " << night_id
        //    << " val " << snsosp_output.solution.profit()
        //    << std::endl;

        // Retrieve column.
        Column column;
        column.row_indices.push_back(night_id);
        column.row_coefficients.push_back(1);
        for (starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::TargetId snsosp_observation_pos = 0;
                snsosp_observation_pos < snsosp_output.solution.number_of_observations();
                ++snsosp_observation_pos) {
            const auto& snsosp_observation = snsosp_output.solution.observation(snsosp_observation_pos);
            ObservableId observable_id = snsosp2sosp_[snsosp_observation.target_id].first;
            Counter observation_time_pos = snsosp2sosp_[snsosp_observation.target_id].second[snsosp_observation.observation_time_pos];
            const Observable& observable = instance_.observable(night_id, observable_id);
            column.row_indices.push_back(
                    instance_.number_of_nights() + observable.target_id);
            column.row_coefficients.push_back(1);
            column.objective_coefficient += observable.profits[observation_time_pos];
        }
        // Extra.
        ColumnExtra extra {night_id, snsosp_output.solution, snsosp2sosp_};
        column.extra = std::shared_ptr<void>(new ColumnExtra(extra));
        columns.push_back(column);
    }
    return columns;
}

Solution columns2solution(
        const Instance& instance,
        const std::vector<std::pair<Column, Value>>& columns)
{
    std::vector<std::vector<std::tuple<ObservableId, Counter, Time>>> sol(instance.number_of_nights());
    for (const auto& colval: columns) {
        std::shared_ptr<ColumnExtra> extra
            = std::static_pointer_cast<ColumnExtra>(colval.first.extra);
        for (starobservationschedulingsolver::flexiblesinglenightstarobservationscheduling::TargetId snsosp_observation_pos = 0;
                snsosp_observation_pos < extra->snsosp_solution.number_of_observations();
                ++snsosp_observation_pos) {
            const auto& snsosp_observation = extra->snsosp_solution.observation(snsosp_observation_pos);
            ObservableId observable_id = extra->snsosp2sosp[snsosp_observation.target_id].first;
            Counter observation_time_pos = extra->snsosp2sosp[snsosp_observation.target_id].second[snsosp_observation.observation_time_pos];
            sol[extra->night_id].push_back({
                    observable_id,
                    observation_time_pos,
                    snsosp_observation.start_time});
        }
    }

    Solution solution(instance);
    for (NightId night_id = 0;
            night_id < instance.number_of_nights();
            ++night_id) {
        sort(sol[night_id].begin(), sol[night_id].end(),
                [](
                    const std::tuple<ObservableId, Counter, Time>& os1,
                    const std::tuple<ObservableId, Counter, Time>& os2) -> bool
                {
                    return std::get<2>(os1) < std::get<2>(os2);
                });
        for (const auto& os: sol[night_id]) {
            solution.append_observation(
                    night_id,
                    std::get<0>(os),
                    std::get<1>(os),
                    std::get<2>(os));
        }
    }

    return solution;
}

}

ColumnGenerationGreedyOutput starobservationschedulingsolver::flexiblestarobservationscheduling::column_generation_greedy(
        const Instance& instance,
        ColumnGenerationOptionalParameters parameters)
{
    init_display(instance, parameters.info);
    parameters.info.os()
            << "Algorithm" << std::endl
            << "---------" << std::endl
            << "Column generation heuristic - greedy" << std::endl
            << std::endl
            << "Parameters" << std::endl
            << "----------" << std::endl
            << "Linear programming solver:  " << parameters.linear_programming_solver << std::endl
            << std::endl;

    ColumnGenerationGreedyOutput output(instance, parameters.info);

    columngenerationsolver::Parameters p = get_parameters(instance);
    columngenerationsolver::GreedyOptionalParameters op;
    op.info.set_time_limit(parameters.info.remaining_time());
    //op.info.set_verbosity_level(1);
    op.column_generation_parameters.linear_programming_solver
        = columngenerationsolver::s2lps(parameters.linear_programming_solver);
    auto greedy_output = columngenerationsolver::greedy(p, op);

    output.update_bound(
            std::ceil(greedy_output.bound - FFOT_TOL),
            std::stringstream(""),
            parameters.info);
    if (greedy_output.solution.size() > 0) {
        output.update_solution(
                columns2solution(instance, greedy_output.solution),
                std::stringstream(""),
                parameters.info);
    }

    output.algorithm_end(parameters.info);
    return output;
}

