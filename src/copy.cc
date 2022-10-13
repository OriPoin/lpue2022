#include "copy.h"
#include <iostream>
#include <filesystem>
#include <bitset>
#include <fstream>
#include "boost/log/trivial.hpp"

/* copy from src_file to des_file return 1 if failed */
int copy(std::string src_file, std::string des_file)
{
    std::fstream src_stream(src_file, std::fstream::in | std::fstream::binary);
    std::fstream des_stream(des_file, std::fstream::out | std::fstream::binary);
    if (!src_stream)
    {
        // check read permissions of source directory
        std::filesystem::perms  src_dir_perms = std::filesystem::status(std::filesystem::path(src_file).parent_path()).permissions();
        if ((src_dir_perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Directory of source file havn't read perm";
            return 1;
        }
        // check exist
        if (!std::filesystem::exists(src_file))
        {
            BOOST_LOG_TRIVIAL(fatal) << "Source file doesn't exist";
            return 1;
        }
        // check read permissions of source file
        std::filesystem::perms  src_perms = std::filesystem::status(src_file).permissions();
        if ((src_perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Directory of source file havn't read perm";
            return 1;
        }
        return 1;
    }
    if (!des_stream)
    {
        // check write permissions of dest directory
        std::filesystem::perms  des_dir_perms = std::filesystem::status(std::filesystem::path(des_file).parent_path()).permissions();
        // x mask
        if ((des_dir_perms & std::filesystem::perms::owner_exec) == std::filesystem::perms::none)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Directory of destination file cannot access";
            return 1;
        }
        // w mask
        if ((des_dir_perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Directory of destination file havn't write perm";
            return 1;
        }
        // check write permissions of dest file
        std::filesystem::perms  des_perms = std::filesystem::status(des_file).permissions();
        if ((des_perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Destination file havn't write perm";
            return 1;
        }
        return 1;
    }
    // Impletion in std is std::fstream:copy (OvO
    des_stream << src_stream.rdbuf();
    return 0;
}
