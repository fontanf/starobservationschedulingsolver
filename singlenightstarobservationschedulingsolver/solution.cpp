#include "singlenightstarobservationschedulingsolver/solution.hpp"

#include "optimizationtools/utils/utils.hpp"

using namespace singlenightstarobservationschedulingsolver;

Solution::Solution(
        const Instance& instance):
    instance_(&instance),
    targets_(instance.number_of_targets(), 0)
{  }

void Solution::append_observation(
        TargetId target_id,
        Time start_time)
{
    const Target& target = instance().target(target_id);
    if (target.release_date > start_time) {
        throw std::runtime_error(
                "target.release_date > start_time");
    }
    if (target.deadline < start_time + target.observation_time) {
        throw std::runtime_error(
                "target.deadline < start_time + target.observation_time");
    }
    if (start_time < current_time_) {
        throw std::runtime_error(
                "start_time < current_time_");
    }
    if (targets_[target_id] == 1) {
        throw std::runtime_error(
                "targets_[target_id] == 1");
    }

    Observation observation;
    observation.target_id = target_id;
    observation.start_time = start_time;
    observations_.push_back(observation);

    targets_[target_id] = 1;
    current_time_ = start_time + target.observation_time;
    profit_ += instance().target(target_id).profit;
}

std::ostream& Solution::print(
        std::ostream& os,
        int verbose) const
{
    if (verbose >= 1) {
        os
            << "Number of targets:  " << optimizationtools::Ratio<TargetId>(number_of_observations(), instance().number_of_targets()) << std::endl
            << "Feasible:           " << feasible() << std::endl
            << "Profit:             " << optimizationtools::Ratio<Profit>(profit(), instance().total_profit()) << std::endl
            ;
    }

    if (verbose >= 2) {
        os << std::endl
            << std::setw(12) << "Target"
            << std::setw(12) << "Start"
            << std::setw(12) << "End"
            << std::endl
            << std::setw(12) << "------"
            << std::setw(12) << "-----"
            << std::setw(12) << "---"
            << std::endl;
        for (TargetId observation_pos = 0;
                observation_pos < number_of_observations();
                ++observation_pos) {
            const Observation& observation = this->observation(observation_pos);
            const Target& target = instance().target(observation.target_id);
            os
                << std::setw(12) << observation.target_id
                << std::setw(12) << observation.start_time
                << std::setw(12) << observation.start_time + target.observation_time
                << std::endl;
        }
    }

    return os;
}

void Solution::write(std::string certificate_path) const
{
    if (certificate_path.empty())
        return;
    std::ofstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                "singlenightstarobservationschedulingsolver::Solution::write\n"
                "Unable to open file \"" + certificate_path + "\".");
    }

    file << observations_.size() << std::endl;
    for (const auto& observation: observations_)
        file << " " << observation.target_id;
    file << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Output ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Output::Output(
        const Instance& instance,
        optimizationtools::Info& info):
    solution(instance),
    bound(instance.total_profit())
{
    info.os()
        << std::setw(12) << "T (s)"
        << std::setw(16) << "LB"
        << std::setw(16) << "UB"
        << std::setw(16) << "GAP"
        << std::setw(12) << "GAP (%)"
        << std::setw(24) << "Comment"
        << std::endl
        << std::setw(12) << "-----"
        << std::setw(16) << "--"
        << std::setw(16) << "--"
        << std::setw(16) << "---"
        << std::setw(12) << "-------"
        << std::setw(24) << "-------"
        << std::endl;
    print(info, std::stringstream(""));
}

void Output::print(
        optimizationtools::Info& info,
        const std::stringstream& s) const
{
    std::string solution_value = optimizationtools::solution_value(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit());
    double absolute_optimality_gap = optimizationtools::absolute_optimality_gap(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit(),
            bound);
    double relative_optimality_gap = optimizationtools::relative_optimality_gap(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit(),
            bound);
    double time = info.elapsed_time();
    std::streamsize precision = std::cout.precision();

    info.os()
        << std::setw(12) << std::fixed << std::setprecision(3) << time << std::defaultfloat << std::setprecision(precision)
        << std::setw(16) << solution_value
        << std::setw(16) << bound
        << std::setw(16) << absolute_optimality_gap
        << std::setw(12) << std::fixed << std::setprecision(2) << relative_optimality_gap * 100 << std::defaultfloat << std::setprecision(precision)
        << std::setw(24) << s.str()
        << std::endl;

    if (!info.output->only_write_at_the_end)
        info.write_json_output();
}

void Output::update_solution(
        const Solution& solution_new,
        const std::stringstream& s,
        optimizationtools::Info& info)
{
    info.lock();

    if (solution.profit() < solution_new.profit()) {
        solution = solution_new;
        print(info, s);

        std::string solution_value = optimizationtools::solution_value(
                optimizationtools::ObjectiveDirection::Maximize,
                solution.feasible(),
                solution.profit());
        double time = info.elapsed_time();

        info.output->number_of_solutions++;
        std::string sol_str = "Solution" + std::to_string(info.output->number_of_solutions);
        info.add_to_json(sol_str, "Value", solution_value);
        info.add_to_json(sol_str, "Time", time);
        info.add_to_json(sol_str, "String", s.str());
        if (!info.output->only_write_at_the_end) {
            info.write_json_output();
            solution.write(info.output->certificate_path);
        }
    }

    info.unlock();
}

void Output::update_bound(
        Profit bound_new,
        const std::stringstream& s,
        optimizationtools::Info& info)
{
    if (bound <= bound_new)
        return;

    info.lock();

    if (bound > bound_new) {
        bound = bound_new;
        print(info, s);

        double time = info.elapsed_time();

        info.output->number_of_bounds++;
        std::string sol_str = "Bound" + std::to_string(info.output->number_of_bounds);
        info.add_to_json(sol_str, "Bound", bound);
        info.add_to_json(sol_str, "Time", time);
        info.add_to_json(sol_str, "String", s.str());
        if (!info.output->only_write_at_the_end)
            solution.write(info.output->certificate_path);
    }

    info.unlock();
}

Output& Output::algorithm_end(
        optimizationtools::Info& info)
{
    std::string solution_value = optimizationtools::solution_value(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit());
    double absolute_optimality_gap = optimizationtools::absolute_optimality_gap(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit(),
            bound);
    double relative_optimality_gap = optimizationtools::relative_optimality_gap(
            optimizationtools::ObjectiveDirection::Maximize,
            solution.feasible(),
            solution.profit(),
            bound);
    time = info.elapsed_time();

    info.add_to_json("Solution", "Value", solution_value);
    info.add_to_json("Bound", "Value", bound);
    info.add_to_json("Solution", "Time", time);
    info.add_to_json("Bound", "Time", time);
    info.os()
        << std::endl
        << "Final statistics" << std::endl
        << "----------------" << std::endl
        << "Value:                        " << solution_value << std::endl
        << "Bound:                        " << bound << std::endl
        << "Absolute optimality gap:      " << absolute_optimality_gap << std::endl
        << "Relative optimality gap (%):  " << relative_optimality_gap * 100 << std::endl
        << "Time (s):                     " << time << std::endl
        ;
    print_statistics(info);
    info.os() << std::endl
        << "Solution" << std::endl
        << "--------" << std::endl ;
    solution.print(info.os(), info.verbosity_level());

    info.write_json_output();
    solution.write(info.output->certificate_path);
    return *this;
}
