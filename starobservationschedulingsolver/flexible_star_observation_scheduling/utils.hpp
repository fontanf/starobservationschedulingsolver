#pragma once

#include "starobservationschedulingsolver/flexible_star_observation_scheduling/solution.hpp"
#include "starobservationschedulingsolver/star_observation_scheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexible_star_observation_scheduling
{

Instance sosp2fsosp(
        const starobservationschedulingsolver::star_observation_scheduling::Instance& instance,
        const std::vector<double>& coefs);

Instance sosp2fsosp(
        const starobservationschedulingsolver::star_observation_scheduling::Instance& instance,
        double coef);

}
}
