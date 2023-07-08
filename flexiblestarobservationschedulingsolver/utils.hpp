#pragma once

#include "flexiblestarobservationschedulingsolver/solution.hpp"
#include "starobservationschedulingsolver/solution.hpp"

namespace flexiblestarobservationschedulingsolver
{

Instance sosp2fsosp(
        const starobservationschedulingsolver::Instance& instance,
        const std::vector<double>& coefs);

Instance sosp2fsosp(
        const starobservationschedulingsolver::Instance& instance,
        double coef);

}
