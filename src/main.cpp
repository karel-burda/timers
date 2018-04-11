#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "timers/blocking.h"
#include "timers/periodic.h"
#include "timers/periodic_async.h"

using namespace std::literals::chrono_literals;

void demonstrate_blocking_timer()
{
    std::cout << "demonstrate_blocking_timer()" << std::endl;

    timers::blocking timer_blocking = {};

    std::cout << "demonstrate_blocking_timer(): starting the blocking timer with 5 seconds timeout" << std::endl;

    // passing 5 seconds here, you can pass any chrono literal or std::chrono::duration manually
    timer_blocking.wait(5s);

    // from other thread you can terminate the timer (using timer_blocking.terminate()),
    // although this is not usually the case, since the main aim is the blocking behaviour itself
}

void demonstrate_periodic_timer()
{
    std::cout << "demonstrate_periodic_timer()" << std::endl;

    timers::periodic timer_periodic = { 2s };

    std::cout << "demonstrate_periodic_timer(): Starting blocking periodic timer with 2 seconds interval" << std::endl;
    
    // the start() call is blocking and causes the callback to be called periodically
    // the usual case is to call the stop() from the destructor or from an external entity,
    // for this example we'll just ensure that it's called after some time
    // and we'll use detached thread for this
    {
        std::thread stop_timer_after_some_time([&timer_periodic]()
        {
            std::cout << "demonstrate_periodic_timer(): We'll stop the timer 'artificially' after 15 seconds" << std::endl;
            std::this_thread::sleep_for(15s);
            timer_periodic.stop();
        });
        stop_timer_after_some_time.detach();
    }
    
    timer_periodic.start([]() { std::cout << "Some useful callback" << std::endl; });
}

void demonstrate_periodic_async_timer()
{
    std::cout << "demonstrate_periodic_async_timer()" << std::endl;

    timers::periodic_async timer_periodic_async = { 4s };

    std::cout << "demonstrate_periodic_async_timer(): Starting not blocking periodic timer with 4 seconds interval" << std::endl;
    
    // the start() call here is not blocking
    timer_periodic_async.start([]() { std::cout << "Some useful callback" << std::endl; });

    // as in the demonstrate_periodic_timer(), we'll artificially stop the timer after some given time
    std::cout << "demonstrate_periodic_async_timer(): We'll stop the timer 'artificially' after 15 seconds" << std::endl;
    std::this_thread::sleep_for(15s);
    timer_periodic_async.stop();
}

int main(int /*argc*/, char ** /*argv*/)
{
    demonstrate_blocking_timer();
    demonstrate_periodic_timer();
    demonstrate_periodic_async_timer();

    return EXIT_SUCCESS;
}
