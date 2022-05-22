#ifndef CONVERT_WHITE_HPP
#define CONVERT_WHITE_HPP
#include <string>
#include <unicode/ucnv.h>
#include <unicode/uenum.h>
#include <unicode/unistr.h>
#include <unicode/ucsdet.h>

namespace White
{
    namespace convert
    {
        std::string Convert(const std::string&, const std::string&);
        std::string encode_detection(const std::string&);
        bool format_check(const std::string& format);
    } // namespace convert
} // namespace White

#endif