#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "compute.h"
#include <boost/test/unit_test.hpp>
#include <boost/date_time.hpp>
#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"
#include <cstdlib>
#include <string>

BOOST_AUTO_TEST_SUITE(compute_test)

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

int dump_tests(std::string filename)
{
    std::fstream of(filename, std::fstream::out);
    std::queue<Job *> *compute_req = get_test(2048);
    of << compute_req->front()->a << compute_req->front()->op << compute_req->front()->b << std::endl;
    of.close();
    return 1;
}

BOOST_AUTO_TEST_CASE(num_count)
{
    init_log();
    std::queue<Job *> *compute_req = get_test(2048);
    int num_req = compute_req->size();
    BOOST_LOG_TRIVIAL(trace) << "compute_req size: " << num_req;
    compute c(compute_req, 1, 1);
    std::queue<Job *> *compute_ans = c.run();
    int num_ans = compute_ans->size();
    BOOST_LOG_TRIVIAL(trace) << "ans nums: " << num_ans;
    while (!compute_ans->empty())
    {
        BOOST_LOG_TRIVIAL(debug) << "ans: " << compute_ans->front()->a << compute_ans->front()->op << compute_ans->front()->b << "=" << compute_ans->front()->c;
        compute_ans->pop();
    }
    BOOST_CHECK_EQUAL(num_ans, num_req);
}
BOOST_AUTO_TEST_SUITE_END()
