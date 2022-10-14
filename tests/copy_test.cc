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
    // reset perms
    std::system("chmod 644 src_file");
    std::system("chmod 644 des_file");
    std::system("dd if=/dev/zero of=src_file count=1024 bs=1024");
    copy(std::string("src_file"), std::string("des_file"));
    BOOST_CHECK_EQUAL(std::system("diff src_file des_file"), 0);
}
BOOST_AUTO_TEST_CASE(no_read_permissions)
{
    std::system("chmod 000 src_file");
    int r = copy(std::string("src_file"), std::string("des_file"));
    BOOST_CHECK_EQUAL(r, 1);
}
BOOST_AUTO_TEST_CASE(no_write_permissions)
{
    std::system("chmod 644 src_file");
    std::system("chmod 000 des_file");
    int r = copy(std::string("src_file"), std::string("des_file"));
    BOOST_CHECK_EQUAL(r, 1);
}
BOOST_AUTO_TEST_CASE(dir_no_read_permissions)
{
    std::system("mkdir -p src_dir");
    std::system("touch src_dir/src_file");
    std::system("chmod 000 src_dir");
    int r = copy(std::string("src_dir/src_file"), std::string("des_file"));
    BOOST_CHECK_EQUAL(r, 1);
}
BOOST_AUTO_TEST_CASE(dir_no_write_permissions)
{
    std::system("chmod 755 src_dir");
    std::system("chmod 755 des_dir");
    std::system("mkdir -p des_dir");
    std::system("touch des_dir/des_file");
    std::system("chmod 000 des_dir");
    int r = copy(std::string("src_file"), std::string("des_dir/des_file"));
    BOOST_CHECK_EQUAL(r, 1);
}
BOOST_AUTO_TEST_CASE(no_src_file)
{
    int r = copy(std::string("src_file_does_not_exist"), std::string("des_dir/des_file"));
    BOOST_CHECK_EQUAL(r, 1);
}
BOOST_AUTO_TEST_SUITE_END()
