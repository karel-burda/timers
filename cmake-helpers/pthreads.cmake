# This macro is meant to be executed only after target is created
macro(link_pthreads_if_needed)
    if (CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_GNUCC)
        message(STATUS "gnu compiler detected, trying to link pthreads")
        find_package (Threads)
        target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
    endif()
endmacro()

