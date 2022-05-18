#include "Semaphore.hpp"

White::semaphore::semaphore(const int& initial, const int& max) :
    count(initial), max(max)
{
}

void White::semaphore::signal()
{
    std::unique_lock<std::mutex> loc(this->m_mutex);
    if(++count == max)
        this->m_cv.notify_all();
}

void White::semaphore::wait()
{
    std::unique_lock<std::mutex> loc(this->m_mutex);
    if(count != max)
        this->m_cv.wait(loc);
}