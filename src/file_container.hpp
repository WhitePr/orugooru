#ifndef WHITE_FILE_CONT_HPP
#define WHITE_FILE_CONT_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

namespace White
{
    class file_container
    {
    private:
        std::vector<std::string> vec;
        std::string              output;
        std::string              format;
        bool                     overwrite;
        bool                     is_dir;

        void add_vec(const std::string&);
        // size:相对于vec中的位置
        // 未来会做多线程支持
        void start_thread(const std::size_t& start,
                          const std::size_t& end) = delete;

    public:
        file_container()  = delete;
        ~file_container() = default;
        file_container(const std::string& input,
                       const std::string& output,
                       const std::string& format,
                       const bool&        overwrite,
                       const std::size_t& thread);
        void run();
    };
} // namespace White

#endif
