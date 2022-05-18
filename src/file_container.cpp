#include "file_container.hpp"
#include "tool.hpp"
#include "convert.hpp"
#include <thread>

White::file_container::file_container(const std::string& input,
                                      const std::string& output,
                                      const std::string& format,
                                      const bool&        overwrite,
                                      const std::size_t& thread) :
    vec(),
    output(output),
    format(format),
    overwrite(overwrite),
    thread(thread),
    is_dir(false)
{
    if (std::filesystem::is_directory(input))
    {
        this->is_dir = true;
        std::cout << "添加文件中" << std::endl;
    }
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
    if (this->vec.empty())
        throw std::runtime_error(
            "待转换的队列为空,"
            "请检查您的输入文件是否是文本文件或包含文本文件的文件夹");
    if (this->is_dir)
    {
        std::size_t              thread_file = this->vec.size() / this->thread;
        std::size_t              remainder   = this->vec.size() % this->thread;
        std::size_t              count       = 0;
        std::vector<std::thread> threads;

        for (std::size_t index = 0; index != remainder; ++index)
        {
            threads.emplace_back(&White::file_container::start_thread,
                                 this,
                                 count,
                                 count + (thread_file + 1));
            count += (thread_file + 1);
        }
        if (thread_file != 0)
        {
            for (std::size_t index = remainder; index != this->thread; ++index)
            {
                threads.emplace_back(&White::file_container::start_thread,
                                     this,
                                     count,
                                     count + thread_file);
                count += thread_file;
            }
        }

        sema.reset(new White::semaphore(0, threads.size()));
        for (auto& item : threads)
            item.detach();


        std::cout << "转换中" << std::endl;
        sema->wait();
    }
    else
    {
        std::cout << "转换中" << std::endl;
        if (this->overwrite)
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->vec[0]);
        else if (output != "")
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->output);
        else
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->vec[0] + '.' + format);
    }
}

void White::file_container::start_thread(const std::size_t& start,
                                         const std::size_t& end)
{
    for (auto index = start; index != end; ++index)
    {
        if (this->overwrite)
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[index]),
                    this->format),
                this->vec[index]);
        else if (output != "")
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[index]),
                    this->format),
                this->output);
        else
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->vec[index] + '.' + format);
    }
    sema->signal();
}