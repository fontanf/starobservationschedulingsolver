#include "starobservationschedulingsolver/algorithms/algorithms.hpp"

#include <boost/program_options.hpp>

using namespace starobservationschedulingsolver;
namespace po = boost::program_options;

Output starobservationschedulingsolver::run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info)
{
    (void)initial_solution;
    (void)generator;

    std::vector<std::string> algorithm_args = po::split_unix(algorithm);
    std::vector<char*> algorithm_argv;
    for (Counter i = 0; i < (Counter)algorithm_args.size(); ++i)
        algorithm_argv.push_back(const_cast<char*>(algorithm_args[i].c_str()));

    if (algorithm.empty() || algorithm_args[0].empty()) {
        throw std::invalid_argument("Missing algorithm.");

    } else if (algorithm_args[0] == "column_generation") {
        ColumnGenerationOptionalParameters parameters;
        parameters.info = info;
        return column_generation_greedy(instance, parameters);

    } else if (algorithm_args[0] == "benders_decomposition") {
        BendersDecompositionOptionalParameters parameters;
        parameters.info = info;
        return benders_decomposition(instance, parameters);

    } else {
        throw std::invalid_argument(
                "Unknown algorithm \"" + algorithm_args[0] + "\".");
    }
}

