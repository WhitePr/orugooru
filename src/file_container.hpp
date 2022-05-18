#ifndef WHITE_FILE_CONT_HPP
#define WHITE_FILE_CONT_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Semaphore.hpp"

namespace White
{
    class file_container
    {
    private:
        std::vector<std::string>          vec;
        std::string                       output;
        std::string                       format;
        bool                              overwrite;
        std::size_t                       thread;
        bool                              is_dir;
        std::unique_ptr<White::semaphore> sema;


        void add_vec(const std::string&);
        void start_thread(const std::size_t& start, const std::size_t& end);

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
