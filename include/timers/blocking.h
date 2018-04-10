#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

namespace timers
{
class blocking
{
public:
    blocking() = default;
    blocking(const blocking &) = delete;
    blocking(blocking &&) = delete;
    blocking & operator=(const blocking &) = delete;
    blocking & operator=(blocking &&) = delete;

    /// Waits and blocks current thread until the "time" elapses OR client code calls "terminate()"
    /// Returns false if terminated by the client
    bool wait_for_termination(std::chrono::duration<double> time)
    {
        std::unique_lock<decltype(m_protection)> lock{ m_protection };

        return !m_cv.wait_for(lock, time, [&]
        {
            return m_terminated_by_client;
        });
    }

    /// Terminates timer, will set the "m_terminated_by_client" to true, so that the cv stops to block
    void terminate()
    {
        {
            std::lock_guard<decltype(m_protection)> lock{ m_protection };

            m_terminated_by_client = true;
        }

        m_cv.notify_all();
    }

private:
    std::condition_variable m_cv;
    mutable std::mutex m_protection;
    bool m_terminated_by_client = false;
};
}
