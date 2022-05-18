#include "file_container.hpp"
#include "tool.hpp"
#include "convert.hpp"
#include <thread>

White::file_container::file_container(const std::string& input,
                                      const std::string& output,
                                      const std::string& format,
                                      const bool&        overwrite,
                                      const std::size_t& thread) :
    vec(), output(output), format(format), overwrite(overwrite), is_dir(false)
{
    if (std::filesystem::is_directory(input))
        this->is_dir = true;
    add_vec(input);
}

void White::file_container::add_vec(const std::string& path)
{
    if (std::filesystem::is_directory(path))
    {
        for (const auto& item : std::filesystem::directory_iterator(path))
        {
            if (std::filesystem::is_directory(item))
            {
                this->add_vec(item.path().c_str());
            }
            else
            {
                auto filename = item.path().c_str();
                if (White::Tool::is_text(filename))
                    this->vec.push_back(filename);
            }
        }
    }
    else
    {
        if (White::Tool::is_text(path))
            this->vec.push_back(path);
    }
}

void White::file_container::run()
{
    if(this->vec.empty())
        throw std::runtime_error("待转换的队列为空,请检查您的输入文件是否是文本文件");
    if (this->is_dir)
    {
        for (const auto& path : this->vec)
        {
            if (this->overwrite)
                White::Tool::write_to_file(
                    White::convert::Convert(White::Tool::read_to_string(path),
                                            this->format),
                    path);

            else
                White::Tool::write_to_file(
                    White::convert::Convert(White::Tool::read_to_string(path),
                                            this->format),
                    path + '.' + format);
        }
    }
    else
    {
        if (this->overwrite)
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->output);
        else
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->output + '.' + format);
    }
}