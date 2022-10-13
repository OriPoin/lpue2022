#include "copy.h"
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
    std::string src_file;
    std::string des_file;
    // Configure options decription
    boost::program_options::options_description opt_desc;
    // Just need help: copy ./src_path/filename ./dest_dir/fiename
    opt_desc.add_options()("help,h", "print this help \ncopy ./src_path/filename ./dest_dir/fiename")("src_file", bpo::value(&src_file), "copy from source file")("des_file", bpo::value(&des_file), "to destination file");
    // Configure positional options decription
    boost::program_options::positional_options_description pos_desc;
    pos_desc.add("src_file", 1);
    pos_desc.add("des_file", 1);
    // Parse command line arguments
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(opt_desc).positional(pos_desc).run(), vm);
    bpo::notify(vm);
    // Check if there are enough args or if --help is given
    if (vm.count("help") || !vm.count("src_file") || !vm.count("des_file"))
    {
        std::cerr << opt_desc << "\n";
        return 1;
    }

    init_log();
    BOOST_LOG_TRIVIAL(debug) << "Boot.log started";
    // Convert boost::program_options::variables_map to std::string
    BOOST_LOG_TRIVIAL(trace) << "copy " << vm["src_file"].as<std::string>() << " " << vm["des_file"].as<std::string>() << std::endl;

    copy(src_file, des_file);

    return 0;
}
