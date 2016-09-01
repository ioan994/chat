#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

class ini_file
{
public:
   ini_file(const std::string& filename);
   std::string get_string(const std::string& section, const std::string& enry);

private:
   boost::property_tree::ptree ptree_;
};

