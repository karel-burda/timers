#pragma once

#include "timers/periodic.h"

#include <future>

namespace timers
{
class periodic_async : public periodic
{
public:
    periodic_async() = delete;
    periodic_async(std::chrono::duration<double> interval)
    : periodic{ interval }
    {

    }
    periodic_async(const periodic_async &) = delete;
    periodic_async(periodic_async &&) = delete;
    periodic_async & operator=(const periodic_async &) = delete;
    periodic_async & operator=(periodic_async &&) = delete;

    void start(std::function<void()> callback) override
    {
        m_async_task = std::async(std::launch::async, [this, callback]
        {
            periodic::start(callback);
        });
    }

    void stop() override
    {
        periodic::stop();

        m_async_task.get();
    }

private:
    std::future<void> m_async_task;
};
}
