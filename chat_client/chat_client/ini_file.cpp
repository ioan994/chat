#include "stdafx.h"

#include <boost/property_tree/ini_parser.hpp>

#include "ini_file.h"

ini_file::ini_file(const std::string& filename)
{
   boost::property_tree::ini_parser::read_ini(filename, ptree_);
}

std::string ini_file::get_string(const std::string& section, const std::string& enry)
{
   return ptree_.get<std::string>(section + "." + enry);
}