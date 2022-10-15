#include "compute.h"
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

std::queue<Job *> *get_test(int size)
{
    std::queue<Job *> *compute_req = new std::queue<Job *>;
    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            Job *job = new Job{.a = (double)j, .b = (double)k, .op = '+'};
            compute_req->push(job);
        }
    }
    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            Job *job = new Job{.a = (double)j, .b = (double)k, .op = '-'};
            compute_req->push(job);
        }
    }
    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            Job *job = new Job{.a = (double)j, .b = (double)k, .op = '*'};
            compute_req->push(job);
        }
    }
    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            Job *job = new Job{.a = (double)j, .b = (double)k, .op = '/'};
            compute_req->push(job);
        }
    }
    return compute_req;
}

int main(int argc, char const **argv)
{
    std::string num;
    // Configure options decription
    boost::program_options::options_description opt_desc;
    // Just need help: copy ./src_path/filename ./dest_dir/fiename
    opt_desc.add_options()("help,h", "print this help \n compute num ")("filename", bpo::value(&num), "cal 4*num**2 samples");
    // Configure positional options decription
    boost::program_options::positional_options_description pos_desc;
    pos_desc.add("num", 1);
    // Parse command line arguments
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(opt_desc).positional(pos_desc).run(), vm);
    bpo::notify(vm);
    // Check if there are enough args or if --help is given
    if (vm.count("help") || !vm.count("num"))
    {
        std::cerr << opt_desc << "\n";
        return 1;
    }

    init_log();
    BOOST_LOG_TRIVIAL(debug) << "Boot.log started";
    // Convert boost::program_options::variables_map to std::string
    BOOST_LOG_TRIVIAL(trace) << "compute " << vm["filename"].as<std::string>() << std::endl;

    std::queue<Job *> *compute_req = get_test(atoi(num.c_str()));
    compute c(compute_req, 2, 2);
    c.run();
    return 0;
}
