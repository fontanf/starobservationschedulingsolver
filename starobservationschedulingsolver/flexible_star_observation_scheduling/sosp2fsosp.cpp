#include "starobservationschedulingsolver/flexible_star_observation_scheduling/utils.hpp"

#include "starobservationschedulingsolver/star_observation_scheduling/instance_builder.hpp"

#include <boost/program_options.hpp>

using namespace starobservationschedulingsolver::flexible_star_observation_scheduling;

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    // Parse program options

    std::string instance_path = "";
    std::string format = "default";
    std::string output_path = "";
    std::vector<double> coef = {0.95};

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input,i", po::value<std::string>(&instance_path)->required(), "set input file (required)")
        ("format,f", po::value<std::string>(&format), "set input file format (default: standard)")
        ("output,o", po::value<std::string>(&output_path), "set JSON output file")
        ("coef,c", po::value<std::vector<double>>(&coef)->multitoken(), "")
        ("continuous", "")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (const po::required_option& e) {
        std::cout << desc << std::endl;;
        return 1;
    }

    starobservationschedulingsolver::star_observation_scheduling::InstanceBuilder sosp_instance_builder;
    sosp_instance_builder.read(instance_path, format);
    const starobservationschedulingsolver::star_observation_scheduling::Instance sosp_instance = sosp_instance_builder.build();
    Instance instance = (vm.count("continuous"))?
            sosp2fsosp(sosp_instance, coef[0]):
            sosp2fsosp(sosp_instance, coef);
    instance.write(output_path);

    return 0;
}

