#ifndef WHITE_FILE_CONT_HPP
#define WHITE_FILE_CONT_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Semaphore.hpp"
#include "indicators/include/indicators/indeterminate_progress_bar.hpp"
#include "indicators/include/indicators/cursor_control.hpp"
#include "indicators/include/indicators/termcolor.hpp"
#include "indicators/include/indicators/progress_bar.hpp"

namespace White
{
    class file_container
    {
    private:
        std::vector<std::string>                              vec;
        std::string                                           output;
        std::string                                           format;
        bool                                                  overwrite;
        std::size_t                                           thread;
        bool                                                  is_dir;
        std::unique_ptr<White::semaphore>                     thread_sema;
        std::unique_ptr<indicators::IndeterminateProgressBar> add_file_bar;
        std::unique_ptr<indicators::ProgressBar>              thread_bar;


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
