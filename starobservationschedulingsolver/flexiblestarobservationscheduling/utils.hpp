#pragma once

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/solution.hpp"
#include "starobservationschedulingsolver/starobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblestarobservationscheduling
{

Instance sosp2fsosp(
        const starobservationschedulingsolver::starobservationscheduling::Instance& instance,
        const std::vector<double>& coefs);

Instance sosp2fsosp(
        const starobservationschedulingsolver::starobservationscheduling::Instance& instance,
        double coef);

}
}
