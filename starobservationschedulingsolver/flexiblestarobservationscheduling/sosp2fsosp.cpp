#include "starobservationschedulingsolver/flexiblestarobservationscheduling/utils.hpp"

#include "starobservationschedulingsolver/starobservationscheduling/instance.hpp"

#include <boost/program_options.hpp>

using namespace starobservationschedulingsolver::flexiblestarobservationscheduling;

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

    starobservationschedulingsolver::starobservationscheduling::Instance sosp_instance(instance_path, format);
    Instance instance = (vm.count("continuous"))?
            sosp2fsosp(sosp_instance, coef[0]):
            sosp2fsosp(sosp_instance, coef);
    instance.write(output_path);

    return 0;
}

