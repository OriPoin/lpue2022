#include "contentscan.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "boost/log/trivial.hpp"
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int Get_filenames(std::string root_dir_name, std::vector<std::string> &file_names)
{
    DIR *dir_ptr = opendir(root_dir_name.c_str());
    std::vector<std::string> dir_names;
    // Something wrong, but we need filenames
    if (dir_ptr==NULL)
    {
        if (errno == EACCES)
        {
            BOOST_LOG_TRIVIAL(debug) << "Permission denied";
        }
        else if (errno == ENOENT)
        {
            BOOST_LOG_TRIVIAL(debug) << "Directory does not exist, or dirname is an empty string";
        }
        else if (errno == ENOTDIR)
        {
            BOOST_LOG_TRIVIAL(debug) << root_dir_name << " isn't a directory";
            file_names.push_back(root_dir_name);
        }
        return errno;
    }
    else
    {
        struct dirent* dirent_ptr;
        // 1 or 0, cannot be -1
        while ((dirent_ptr=readdir(dir_ptr))!=0)
        {
            if (strcmp(dirent_ptr->d_name, ".") != 0 && strcmp(dirent_ptr->d_name, "..") != 0)
            {
                Get_filenames(root_dir_name+"/"+dirent_ptr->d_name,file_names);
            }
        }
    }
    return 0;
}

int contentscan(std::string scan_str, std::string scan_dir, std::vector<std::string> &file_names)
{
    // get filenames
    Get_filenames(scan_dir,file_names);
    // Lambda: remember to capture scan_str
    auto end = std::remove_if(file_names.begin(),
                            file_names.end(),
                            [scan_str](std::string &i)
                            {
                                // std::ifstream to std::string by std::ostringstream
                                std::ifstream ifs(i);
                                std::ostringstream sstr;
                                sstr << ifs.rdbuf();
                                // if cannot find scan_str of sstr, b
                                return !(sstr.str().find(scan_str)!=std::string::npos);
                            });
    file_names.erase(end, file_names.end());
    return 0;
}
