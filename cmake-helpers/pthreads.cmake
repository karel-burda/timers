# This macro is meant to be executed only after target is created
macro(link_pthreads_if_available)
    find_package (Threads QUIET)
    message(STATUS "pthreads found, going to link these")
    target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endmacro()

