#pragma once

#include <functional>
#include <stdexcept>

#include "timers/blocking.h"

namespace timers
{
class periodic
{
public:
    class empty_callback_set : public std::runtime_error
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
        while (m_blocking_timer.wait_for_termination(m_interval))
        {
            if (callback)
            {
                callback();
            }
            else
            {
                //throw empty_callback_set{"Empty callback is set"};
            }
        }
    }

    std::chrono::duration<double> m_interval = {0};
    blocking m_blocking_timer = {};
};
}
