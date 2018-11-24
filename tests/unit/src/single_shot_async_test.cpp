#include <future>
#include <mutex>
#include <thread>

#include <gtest/gtest.h>

#include <test_utils/make_all_members_public.hpp>
#include <test_utils/lifetime_assertions.hpp>
#include <test_utils/mutex.hpp>
#include <test_utils/static_class_assertions.hpp>
#include <test_utils/time.hpp>
#include <timers/single_shot_async.hpp>

namespace
{
using namespace std::chrono_literals;

namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class single_shot_async_test : public ::testing::Test
{
private:
    void callback()
    {
        m_callback_called = true;
    }

    timers::single_shot_async m_timer;
    bool m_callback_called = false;
};

TEST(single_shot_async_construction_destruction, construction_destruction)
{
    test_utils::assert_construction_and_destruction<timers::single_shot_async>();
}

TEST_F(single_shot_async_test, static_assertions)
{
    test_utils::assert_default_constructibility<timers::single_shot_async, true>();
    test_utils::assert_copy_constructibility<timers::single_shot_async, false>();
    test_utils::assert_move_constructibility<timers::single_shot_async, false>();

    SUCCEED();
}

TEST_F(single_shot_async_test, default_values)
{
    EXPECT_FALSE(m_timer.m_async_task.valid());
    test_utils::check_if_mutex_is_owned(m_timer.m_async_protection, false);
    test_utils::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}

TEST_F(single_shot_async_test, callback_called)
{
    m_timer.start(2s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(3s);

    EXPECT_FALSE(m_timer.blocking::m_terminate_forcefully);
    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_async_test, start_exception_policy_stop)
{
    EXPECT_NO_THROW(m_timer.start(1s, [](){ throw std::exception{}; }, timers::policies::start::exception::stop));

    std::this_thread::sleep_for(3s);

    test_utils::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    test_utils::check_if_mutex_is_owned(m_timer.m_async_protection, false);
    test_utils::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
}

TEST_F(single_shot_async_test, callback_multiple_times)
{
    using clock = std::chrono::steady_clock;

    const auto start = clock::now();
    auto end = clock::now();

    m_timer.start(1s, [&end]()
    {
        end = clock::now();
    });
    m_timer.start(2s, [&end]()
    {
        end = clock::now();
    });
    m_timer.start(2s, [&end]()
    {
        end = clock::now();
    });

    std::this_thread::sleep_for(5s);

    test_utils::assert_that_elapsed_time_in_tolerance(std::chrono::duration_cast<std::chrono::seconds>(end - start), 5s, 100s);
}

TEST_F(single_shot_async_test, start_in_parallel)
{
    bool taskFinished1 = false;
    bool taskFinished2 = false;

    std::condition_variable m_cv;
    std::mutex m_cv_protection;

    m_timer.start(2s, [&taskFinished1, &m_cv_protection, &m_cv]()
    {
        std::lock_guard<decltype(m_cv_protection)> lock { m_cv_protection };

        taskFinished1 = true;
        m_cv.notify_one();
    });
    m_timer.start(2s, [&taskFinished2, &m_cv_protection, &m_cv]()
    {
        std::lock_guard<decltype(m_cv_protection)> lock { m_cv_protection };

        taskFinished2 = true;
        m_cv.notify_one();
    });

    std::unique_lock<decltype(m_cv_protection)> cv_lock{ m_cv_protection };

    const auto expired = m_cv.wait_for(cv_lock, 10s, [&]
    {
        return taskFinished1 && taskFinished2;
    });

    ASSERT_TRUE(expired);
    EXPECT_TRUE(taskFinished1 && taskFinished2);
}

TEST_F(single_shot_async_test, stop)
{
    // deliberately calling stop() on "empty" std::future
    m_timer.stop();
    m_timer.stop();

    m_timer.start(4s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(1s);

    m_timer.stop();

    EXPECT_TRUE(m_timer.blocking::m_terminate_forcefully);
    EXPECT_FALSE(m_callback_called);
}
}
