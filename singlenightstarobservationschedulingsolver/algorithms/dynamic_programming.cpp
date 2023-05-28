#include "singlenightstarobservationschedulingsolver/algorithms/dynamic_programming.hpp"

using namespace singlenightstarobservationschedulingsolver;

struct DynamicProgrammingState
{
    Time time;
    Profit profit;
    TargetId target_id;
    DynamicProgrammingState* prev;
};

inline std::ostream& operator<<(std::ostream &os, const DynamicProgrammingState& s)
{
    os << "time " << s.time << " profit " << s.profit;
    return os;
}

Output singlenightstarobservationschedulingsolver::dynamic_programming(
        const Instance& instance,
        DynamicProgrammingOptionalParameters parameters)
{

    init_display(instance, parameters.info);
    parameters.info.os()
        << "Algorithm" << std::endl
        << "---------" << std::endl
        << "Dynamic programming" << std::endl
        << std::endl;

    Output output(instance, parameters.info);

    TargetId n = instance.number_of_targets();
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
    std::vector<TargetId> sorted_targets(n);
    std::iota(sorted_targets.begin(), sorted_targets.end(), 0);
    sort(sorted_targets.begin(), sorted_targets.end(),
            [&instance](TargetId target_id_1, TargetId target_id_2) -> bool
            {
                return instance.target(target_id_1).release_date
                    + instance.target(target_id_1).deadline
                    < instance.target(target_id_2).release_date
                    + instance.target(target_id_2).deadline;
            });

    // Compute states.
    std::vector<std::vector<DynamicProgrammingState>> states(n + 1);
    states[0].push_back({0, 0, -1, nullptr});
    for (TargetId target_pos = 0; target_pos < n; ++target_pos) {
        TargetId target_id = sorted_targets[target_pos];
        const Target& target = instance.target(target_id);
        //std::cout << "target_pos " << target_pos
        //    << " j " << j
        //    << " m " << (double)(rj + dj) / 2
        //    << " states[target_pos].size() " << states[target_pos].size()
        //    << std::endl;
        auto it  = states[target_pos].begin();
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
    for (const DynamicProgrammingState& s: states[n])
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

    output.update_solution(solution, std::stringstream(""), parameters.info);
    output.update_bound(solution.profit(), std::stringstream(""), parameters.info);

    return output.algorithm_end(parameters.info);
}
