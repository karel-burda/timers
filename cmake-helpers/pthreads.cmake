# This macro is meant to be executed only after target is created
macro(_link_pthreads_if_available)
    find_package (Threads QUIET)

    if (Threads_FOUND)
        _print_status_message("pthreads found, going to link these")
        target_link_libraries (${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
     endif()
endmacro()
