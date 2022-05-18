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
    auto thread    = command.second["thread"].as<std::size_t>();

    std::cout << "转换中" << std::endl;

    try
    {
        White::file_container(input, output, format, overwrite, thread).run();
    }
    catch (const std::runtime_error& x)
    {
        std::cerr << "\033[31m"
                  << "Error:"
                  << "\033[0m" << x.what() << std::endl;
    }


    return 0;
}