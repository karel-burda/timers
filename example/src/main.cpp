/// This example is deliberately not using "chrono_literals", because this .cpp gets compiled in both C++11 and C++14
/// std::endl is being used instead of "\n" because we want to always flush IO because this gets run with vagrind

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "timers/blocking.h"
#include "timers/periodic.h"
#include "timers/periodic_async.h"
#include "timers/single_shot.h"
#include "timers/single_shot_async.h"

namespace timers = burda::timers;

void demonstrate_blocking_timer()
{
    std::cout << "demonstrate_blocking_timer()" << std::endl;

    timers::blocking timer;

    std::cout << "demonstrate_blocking_timer(): starting the blocking timer with 5 seconds timeout" << std::endl;

    // passing 5 seconds here, you can pass any chrono literal or std::chrono::duration manually
    timer.block(std::chrono::seconds{ 5 });

    // from other thread you can terminate the timer (using timer.stop()),
    // although this is not usually the case, since the main aim is the blocking behaviour itself
}

void demonstrate_single_shot_timer()
{
    std::cout << "demonstrate_single_shot_timer()" << std::endl;

    timers::single_shot timer;
    timers::single_shot_async timer_async;

    std::cout << "demonstrate_single_shot_timer(): starting the single shot timer with 2 seconds timeout and simple callback" << std::endl;
    // this call is blocking
    timer.start(std::chrono::seconds{ 2 }, []() { std::cout << "demonstrate_single_shot_timer(): Hello from single shot callback" << std::endl; });

    // this call is asynchronous
    std::cout << "demonstrate_single_shot_timer(): starting the single shot async timer with 2 seconds timeout and simple callback" << std::endl;
    timer_async.start(std::chrono::seconds{ 2 }, []() { std::cout << "demonstrate_single_shot_timer(): Hello from single shot async callback" << std::endl; });

    // we could even "stack" the start() commands in a row, e.g.:
    //timer.start(1s, []() { std::cout << "1\n"; });
    //timer.start(2s, []() { std::cout << "1\n"; });
    //timer.start(3s, []() { std::cout << "1\n"; });

    // stop artificially after some time
    std::cout << "demonstrate_single_shot_timer(): We'll stop the single_shot async 'artificially' after 5 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{ 5 });
    std::cout << "demonstrate_single_shot_timer(): Going to stop the async single_shot timer" << std::endl;
    timer_async.stop();
}

void demonstrate_periodic_timer()
{
    std::cout << "demonstrate_periodic_timer()" << std::endl;

    timers::periodic timer;
    timers::periodic_async timer_async;

    std::cout << "demonstrate_periodic_timer(): Starting the periodic timer with 3 seconds interval" << std::endl;

    // the start() call is blocking and causes the callback to be called periodically
    // the usual case is to call the stop() from the destructor or from an external entity,
    // for this example we'll just ensure that it's called after some time
    // and we'll use detached thread for this
    auto stop_timer_after_some_time = std::async(std::launch::async, [&timer]
    {
        std::cout << "demonstrate_periodic_timer(): We'll stop the periodic 'artificially' after 10 seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds{ 10 });
        std::cout << "demonstrate_periodic_timer(): Going to stop the periodic timer" << std::endl;
        timer.stop();
    });
    // blocking call
    timer.start(std::chrono::seconds{ 3 }, []() { std::cout << "demonstrate_periodic_timer(): Hello from periodic callback" << std::endl; });

    std::cout << "demonstrate_periodic_timer(): Starting async periodic timer with 3 seconds interval" << std::endl;
    // asynchronous call
    timer_async.start(std::chrono::seconds{ 3 }, []() { std::cout << "demonstrate_periodic_timer(): Hello from periodic async callback" << std::endl; });

    // stop artificially after some time
    std::cout << "demonstrate_periodic_timer(): We'll stop the periodic async 'artificially' after 10 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{ 10 });
    std::cout << "demonstrate_periodic_timer(): Going to stop the async periodic timer" << std::endl;
    timer_async.stop();
}

int main(int /*argc*/, char ** /*argv*/)
{
    demonstrate_blocking_timer();
    demonstrate_single_shot_timer();
    demonstrate_periodic_timer();

    return EXIT_SUCCESS;
}
