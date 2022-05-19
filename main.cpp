#include "src/file_container.hpp"
#include "src/tool.hpp"

int main(int argc, char* argv[])
{
    auto command = White::Tool::Command(argc, argv);
    if (command.second.count("help"))
    {
        std::cout << command.first << std::endl;
        return 0;
    }
    if (!command.second.count("input"))
    {
        std::cerr << "\033[31m"
                  << "Error:"
                  << "\033[0m"
                  << "无输入文件" << std::endl;
        return EXIT_FAILURE;
    }

    auto input     = command.second["input"].as<std::string>();
    auto output    = command.second["output"].as<std::string>();
    auto format    = command.second["format"].as<std::string>();
    auto overwrite = static_cast<bool>(command.second.count("overwrite"));
    auto thread    = (command.second["thread"].as<std::size_t>() == 0
                          ? std::thread::hardware_concurrency()
                          : command.second["thread"].as<std::size_t>());
    auto match     = command.second["match"].as<std::string>();

    try
    {
        White::file_container(input, output, format, overwrite, thread, match)
            .run();
    }
    catch (const std::runtime_error& x)
    {
        std::cerr << "\033[31m"
                  << "Error:"
                  << "\033[0m" << x.what()
                  << std::endl
                  // 抛出异常时终端样式可能未恢复
                  << termcolor::reset;
        indicators::show_console_cursor(true);
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "\033[31m"
                  << "Error:"
                  << "\033[0m"
                  << "未知错误" << std::endl　 << termcolor::reset;
        indicators::show_console_cursor(true);
        return EXIT_FAILURE;
    }


    return 0;
}