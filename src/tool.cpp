#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <thread>
#include <boost/program_options.hpp>
#ifndef TERMUX
#include <bits/local_lim.h>
#else
#include <bits/posix_limits.h>
#endif
#include "tool.hpp"

namespace bpo = boost::program_options;

std::string White::Tool::read_to_string(const std::string& input)
{
    std::ostringstream out{};
    std::ifstream      file(input);

    out << file.rdbuf();

    return std::string(out.str());
}

void White::Tool::write_to_file(const std::string& input_string,
                                const std::string& output_file)
{
    std::ofstream op(output_file);

    op << input_string;
}

std::pair<bpo::options_description, bpo::variables_map>
    White::Tool::Command(int argc, const char* const argv[])
{
    bpo::options_description opts("All Command");
    bpo::variables_map       vm;
    std::string thread("指定线程\n默认值为设备核心数");

    opts.add_options()("input,i",
                       bpo::value<std::string>(),
                       "输入文件\n可以是文件也可以是文件夹\n文件夹将递归文件夹"
                       "下所有文件\n将自动检测文件是否是文本文件")(
        "output,o",
        bpo::value<std::string>()->default_value(""),
        "输出文件\n不指定将输出给源文件添加转换格式后缀的新文件(比如1.txt->1."
        "txt.utf8)\n输入是文件夹时该项无效")(
        "format,f",
        bpo::value<std::string>()->default_value("utf8"),
        "要转换的编码\n可以在ICU官网查看编码别名\n默认utf8")(
        "overwrite,w", "是否覆盖源文件\n添加该项可以不用指定输出文件")(
        "thread,t",
        bpo::value<std::size_t>()->default_value(
            std::thread::hardware_concurrency()),
        thread.c_str())(
        "match,m",
        bpo::value<std::string>()->default_value(""),
        "要匹配的扩展名(包含. 比如.txt .tar)\n多个扩展名请用;隔开 匹配大小写")(
        "help,h", "显示帮助");

    bpo::store(bpo::parse_command_line(argc, argv, opts), vm);

    return std::make_pair(opts, vm);
}

bool White::Tool::is_text(const std::string& path)
{
    auto                       cookie = ::magic_open(MAGIC_MIME_TYPE);
    std::shared_ptr<::magic_t> Ref_cookie(&cookie,
                                          [](::magic_t* a)
                                          {
                                              ::magic_close(*a);
                                          });
    ::magic_load(cookie, nullptr);
    std::string type(::magic_file(cookie, path.c_str()));

    return type.starts_with("text");
}