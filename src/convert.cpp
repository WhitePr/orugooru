#include <iostream>
#include <memory>
#include <unicode/ucnv.h>
#include <unicode/uenum.h>
#include <unicode/unistr.h>
#include <unicode/ucsdet.h>
#include "convert.hpp"

// Original:原字符串 Target:要转换成的格式 返回转换后字符串
std::string White::convert::Convert(const std::string& Original,
                                    const std::string& Format)
{
    auto original_format = White::convert::encode_detection(Original);
    // 构建ustring
    icu::UnicodeString src(Original.c_str(), original_format.c_str());
    // 转换好的字符串
    std::string result{};
    // 重置字符串大小
    result.resize(src.extract(0, src.length(), nullptr, Format.c_str()));

    src.extract(0, src.length(), &result[0], Format.c_str());

    return result;
}

std::string White::convert::encode_detection(const std::string& input)
{
    auto                              status = U_ZERO_ERROR;
    auto                              csd    = ucsdet_open(&status);
    const UCharsetMatch*              ucm    = nullptr;
    std::shared_ptr<UCharsetDetector> pcsd(csd, ucsdet_close);

    ucsdet_setText(csd, input.c_str(), input.size(), &status);
    ucm = ucsdet_detect(csd, &status);

    return std::string(ucsdet_getName(ucm, &status));
}

bool White::convert::format_check(const std::string& format)
{
    UErrorCode status   = U_ZERO_ERROR;
    auto       count = ucnv_countAliases(format.c_str(), &status);

    return count;
}