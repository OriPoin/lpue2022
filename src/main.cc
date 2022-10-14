#include "contentscan.h"
#include <iostream>
#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

static void init_log()
{
    // Log format
    static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

    // Output message to console
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = COMMON_FMT,
        boost::log::keywords::auto_flush = true);

    // common attributes: IDs, Timestamp and so on
    boost::log::add_common_attributes();

    // Only output message with INFO or higher severity in Release
#ifndef __DEBUG__
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info);
#endif
}

int main(int argc, char const **argv)
{

    init_log();
    BOOST_LOG_TRIVIAL(trace) << "Boot.log started";

    BOOST_LOG_TRIVIAL(trace) << "Boot.program_options processing";

    std::string scan_str;
    std::string scan_dir;
    // Configure options decription
    boost::program_options::options_description opt_desc;
    // Just need help: copy ./src_path/filename ./dest_dir/fiename
    opt_desc.add_options()("help,h", "print this help \ncontentscan \"scan_str\" scan_dir")("scan_str", bpo::value(&scan_str), "copy from source file")("scan_dir", bpo::value(&scan_dir), "to destination file");
    // Configure positional options decription
    boost::program_options::positional_options_description pos_desc;
    pos_desc.add("scan_str", 1);
    pos_desc.add("scan_dir", 1);
    // Parse command line arguments
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(opt_desc).positional(pos_desc).run(), vm);
    bpo::notify(vm);
    // Check if there are enough args or if --help is given
    if (vm.count("help") || !vm.count("scan_str") || !vm.count("scan_dir"))
    {
        std::cerr << opt_desc << "\n";
        return 1;
    }
    BOOST_LOG_TRIVIAL(trace) << "Boot.program_options parsing completed";

    // Convert boost::program_options::variables_map to std::string
    BOOST_LOG_TRIVIAL(debug) << "contentscan " << vm["scan_str"].as<std::string>() << " " << vm["scan_dir"].as<std::string>() << std::endl;

    // Main function
    // get filenames
    std::vector<std::string> file_names;
    contentscan(scan_str, scan_dir, file_names);
    // print
    for (auto i : file_names)
    {
        std::cout << i << std::endl;
    }
    return 0;
}
