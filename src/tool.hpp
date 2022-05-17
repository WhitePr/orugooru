#ifndef TOOL_WHITE_HPP
#define TOOL_WHITE_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <boost/program_options.hpp>
#include <magic.h>


namespace White
{
    namespace Tool
    {
        namespace bpo = boost::program_options;
        std::string read_to_string(const std::string& name);
        void        write_to_file(const std::string& input_string,
                                  const std::string& output_file);

        std::pair<bpo::options_description, bpo::variables_map>
            Command(int, const char* const[]);
        bool is_text(const std::string& path);
    } // namespace Tool
} // namespace White

#endif