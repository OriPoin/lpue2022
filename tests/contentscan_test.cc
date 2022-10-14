#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "contentscan.h"
#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

BOOST_AUTO_TEST_SUITE(content_test)

BOOST_AUTO_TEST_CASE(content_right_case)
{
    std::system("rm -rf test_1");
    std::system("mkdir -p test_1/test_2/test_3");
    std::system("echo 1 >> test_1/1");
    std::system("echo 2 >> test_1/test_2/2");
    std::system("echo 3 >> test_1/test_2/test_3/3");
    std::vector<std::string> file_names;
    contentscan(std::string("2"), std::string("test_1"), file_names);
    BOOST_CHECK_EQUAL(file_names[0], std::string("test_1/test_2/2"));
    std::system("echo 2 >> test_1/test_2/test_3/3");
    std::vector<std::string> file_names_2;
    contentscan(std::string("2"), std::string("test_1"), file_names_2);
    BOOST_CHECK_EQUAL(file_names_2.size(), 2);
}
BOOST_AUTO_TEST_SUITE_END()
