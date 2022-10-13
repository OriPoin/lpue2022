#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "copy.h"
#include <boost/test/unit_test.hpp>
#include <boost/date_time.hpp>
#include <cstdlib>
#include <string>

BOOST_AUTO_TEST_SUITE(copy_test)
BOOST_AUTO_TEST_CASE(content_right)
{
    std::system("dd if=/dev/zero of=src_file count=1024 bs=1024");
    copy(std::string("src_file"), std::string("des_file"));
    BOOST_CHECK_EQUAL(std::system("diff src_file des_file"), 0);
}
BOOST_AUTO_TEST_SUITE_END()
