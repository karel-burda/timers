# This macro is meant to be executed only after target is created
macro(link_pthreads_if_available)
    find_package (Threads QUIET)

    if (Threads_FOUND)
        message(STATUS "timers: pthreads found, going to link these")
        target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
     endif()
endmacro()

