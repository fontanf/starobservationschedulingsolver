#include "starobservationschedulingsolver/single_night_star_observation_scheduling/algorithms/dynamic_programming.hpp"

#include "starobservationschedulingsolver/single_night_star_observation_scheduling/algorithm_formatter.hpp"

using namespace starobservationschedulingsolver::single_night_star_observation_scheduling;

namespace
{

struct DynamicProgrammingState
{
    Time time;
    Profit profit;
    TargetId target_id;
    DynamicProgrammingState* prev;
};

/*
inline std::ostream& operator<<(
        std::ostream& os,
        const DynamicProgrammingState& s)
{
    os << "time " << s.time << " profit " << s.profit;
    return os;
}
*/

}

const Output starobservationschedulingsolver::single_night_star_observation_scheduling::dynamic_programming(
        const Instance& instance,
        const DynamicProgrammingOptionalParameters& parameters)
{
    Output output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Dynamic programming");
    algorithm_formatter.print_header();

    Solution solution(instance);
    //std::cout << "n " << n << std::endl;
    //for (TargetId j = 0; j < n; ++j)
    //    std::cout << "j " << j
    //        << " w " << instance.target(j).profit
    //        << " r " << instance.target(j).r
    //        << " d " << instance.target(j).d
    //        << " p " << instance.target(j).p
    //        << std::endl;

    // Sort targets.
    std::vector<TargetId> sorted_targets(instance.number_of_targets());
    std::iota(sorted_targets.begin(), sorted_targets.end(), 0);
    sort(
            sorted_targets.begin(),
            sorted_targets.end(),
            [&instance](TargetId target_id_1, TargetId target_id_2) -> bool
            {
                return instance.target(target_id_1).meridian
                    < instance.target(target_id_2).meridian;
            });

    // Compute states.
    std::vector<std::vector<DynamicProgrammingState>> states(instance.number_of_targets() + 1);
    states[0].push_back({0, 0, -1, nullptr});
    for (TargetId target_pos = 0;
            target_pos < instance.number_of_targets();
            ++target_pos) {
        TargetId target_id = sorted_targets[target_pos];
        const Target& target = instance.target(target_id);
        //std::cout << "target_pos " << target_pos
        //    << " target_id " << target_id
        //    << " meridian " << target.meridian
        //    << " states[target_pos].size() " << states[target_pos].size()
        //    << std::endl;
        auto it = states[target_pos].begin();
        auto it1 = states[target_pos].begin();
        while (it != states[target_pos].end() || it1 != states[target_pos].end()) {
            if (it1 != states[target_pos].end()
                    && (it == states[target_pos].end()
                        || it->time > std::max(it1->time, target.release_date)
                        + target.observation_time)) {
                DynamicProgrammingState s1 {
                        std::max(it1->time, target.release_date) + target.observation_time,
                        it1->profit + target.profit,
                        target_id,
                        &(*it1) };
                //std::cout << *it1 << " -> " << s1 << std::endl;
                if (s1.time > target.deadline) {
                    it1++;
                    continue;
                }

                if (states[target_pos + 1].empty()
                        || s1.profit > states[target_pos + 1].back().profit) {
                    if (!states[target_pos + 1].empty()
                            && s1.time == states[target_pos + 1].back().time) {
                        states[target_pos + 1].back() = s1;
                    } else {
                        states[target_pos + 1].push_back(s1);
                    }
                }
                it1++;
            } else {
                assert(it != states[target_pos].end());
                //std::cout << *it << std::endl;
                if (states[target_pos + 1].empty()
                        || it->profit > states[target_pos + 1].back().profit) {
                    if (!states[target_pos + 1].empty()
                            && it->time == states[target_pos + 1].back().time) {
                        states[target_pos + 1].back() = *it;
                    } else {
                        states[target_pos + 1].push_back(*it);
                    }
                }
                ++it;
            }
        }
    }

    // Find best state.
    const DynamicProgrammingState* s_best = nullptr;
    for (const DynamicProgrammingState& s: states[instance.number_of_targets()])
        if (s_best == nullptr || s_best->profit < s.profit)
            s_best = &s;
    //std::cout << "s_best t " << s_best.time << " profit " << s_best.profit << std::endl;

    // Retrieve solution.
    const DynamicProgrammingState* s_curr = s_best;
    std::vector<const DynamicProgrammingState*> solution_states;
    while (s_curr->prev != nullptr) {
        solution_states.push_back(s_curr);
        s_curr = s_curr->prev;
    }
    std::reverse(solution_states.begin(), solution_states.end());
    for (const DynamicProgrammingState* state: solution_states) {
        solution.append_observation(
                state->target_id,
                state->time - instance.target(state->target_id).observation_time);
    }

    algorithm_formatter.update_solution(solution, "");
    algorithm_formatter.update_bound(solution.profit(), "");

    algorithm_formatter.end();
    return output;
}
