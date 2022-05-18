#include <thread>
#include <mutex>
#include <condition_variable>

namespace White
{
    class semaphore
    {
    private:
        int                     count, max;
        std::mutex              m_mutex;
        std::condition_variable m_cv;
    public:
        semaphore() = delete;
        ~semaphore() = default;
        semaphore(const int& initial, const int& max);
        void signal();
        void wait();
    };
} // namespace White
