# This macro is meant to be executed only after target is created
macro(_link_threads_if_available)
    find_package(Threads QUIET)

    if (CMAKE_THREAD_LIBS_INIT)
        _print_status_message("threading library found, going to link: ${CMAKE_THREAD_LIBS_INIT}")
        target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
     endif()
endmacro()
