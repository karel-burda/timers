#pragma once

#include <exception>
#include <functional>

#include "timers/blocking.h"

namespace timers
{
class periodic
{
public:
    class callback_not_callable : public std::exception
    {
    };

    periodic() = delete;
    periodic(std::chrono::duration<double> interval)
    : m_interval{ interval }
    {

    }
    periodic(const periodic &) = delete;
    periodic(periodic &&) = delete;
    periodic & operator=(const periodic &) = delete;
    periodic & operator=(periodic &&) = delete;

    virtual void start(std::function<void()> callback)
    {
        perform_callback_periodically(std::move(callback));
    }

    virtual void stop()
    {
        m_blocking_timer.terminate();
    }

private:
    void perform_callback_periodically(std::function<void()> callback)
    {
        while (m_blocking_timer.wait(m_interval))
        {
            if (callback)
            {
                callback();
            }
            else
            {
                throw callback_not_callable{};
            }
        }
    }

    std::chrono::duration<double> m_interval = {0};
    blocking m_blocking_timer = {};
};
}
