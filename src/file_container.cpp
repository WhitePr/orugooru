#include <thread>
#include "file_container.hpp"
#include "tool.hpp"
#include "convert.hpp"
#include <atomic>

White::file_container::file_container(const std::string& input,
                                      const std::string& output,
                                      const std::string& format,
                                      const bool&        overwrite,
                                      const std::size_t& thread,
                                      const std::string& match) :
    vec(),
    output(output),
    format(format),
    overwrite(overwrite),
    thread(thread),
    is_dir(false),
    add_file_bar(nullptr),
    thread_bar(nullptr)
{
    this->write_set(match);
    if (std::filesystem::is_directory(input))
    {
        this->is_dir = true;
        indicators::show_console_cursor(false);
        this->add_file_bar.reset(new indicators::IndeterminateProgressBar{
            indicators::option::BarWidth{60},
            indicators::option::Start{"["},
            indicators::option::Fill{"."},
            indicators::option::Lead{"<==>"},
            indicators::option::End{"]"},
            indicators::option::PostfixText{"添加文件至待处理队列中"},
        });

        // 进度条更新线程
        std::thread(
            [this]()
            {
                while (!this->add_file_bar->is_completed())
                {
                    this->add_file_bar->tick();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            })
            .detach();

        this->add_vec_folder(input);
        this->add_file_bar->mark_as_completed();
        std::cout << termcolor::bold << termcolor::green
                  << "文件队列添加完毕,队列大小:" << this->vec.size()
                  << std::endl
                  << termcolor::reset;
        indicators::show_console_cursor(true);
    }
    else
        this->add_vec_file(input);
}

void White::file_container::add_vec_folder(const std::string& path)
{
    for (const auto& item : std::filesystem::recursive_directory_iterator(path))
        this->add_vec_file(item.path().c_str());
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

        thread_sema.reset(new White::semaphore(0, threads.size()));
        for (auto& item : threads)
            item.detach();

        indicators::show_console_cursor(false);
        this->thread_bar.reset(new indicators::ProgressBar{
            indicators::option::BarWidth{60},
            indicators::option::Start{"["},
            indicators::option::Fill{"="},
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{"]"},
            indicators::option::MaxProgress{this->vec.size()}});

        thread_sema->wait();
        indicators::show_console_cursor(true);
        std::cout << termcolor::bold << termcolor::green << "处理完毕"
                  << std::endl
                  << termcolor::reset;
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
    static std::atomic_size_t conut = 0;
    for (auto index = start; index != end; ++index)
    {
        if (this->overwrite)
        {
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[index]),
                    this->format),
                this->vec[index]);
            ++conut;
            this->thread_bar->set_option(indicators::option::PostfixText(
                std::to_string(conut) + "/" +
                std::to_string(this->vec.size())));
            this->thread_bar->tick();
        }
        else
        {
            White::Tool::write_to_file(
                White::convert::Convert(
                    White::Tool::read_to_string(this->vec[0]), this->format),
                this->vec[index] + '.' + format);
            ++conut;
            this->thread_bar->set_option(indicators::option::PostfixText(
                std::to_string(conut) + "/" +
                std::to_string(this->vec.size())));
            this->thread_bar->tick();
        }
    }
    thread_sema->signal();
}

void White::file_container::add_vec_file(const std::string& path)
{
    auto name = std::filesystem::path(path).extension();
    if (White::Tool::is_text(path))
        this->vec.push_back(path);
    else if (this->extension_name.find(name) != this->extension_name.end())
        this->vec.push_back(path);
}

void White::file_container::write_set(const std::string& str)
{
    std::size_t pos = 0;

    while (pos != std::string::npos)
    {
        auto pos_temp = str.find(';', pos);
        auto name     = str.substr(pos, pos_temp);
        this->extension_name.insert(name);
        pos =
            (pos_temp == std::string::npos ? std::string::npos : pos_temp + 1);
    }
}