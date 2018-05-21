macro(add_pedantic_warning_level)
    set(CXX_WARNING_FLAGS "")

    if (MSVC)
            # "/Wall" Hardly usable on MSVC without excluding stdlib
        list(APPEND CXX_WARNING_FLAGS "/W4")
    else()
        list(APPEND CXX_WARNING_FLAGS "-Wall" "-Werror" "-pedantic" "-Wno-long-long")
    endif()

    foreach(FLAG IN LISTS CXX_WARNING_FLAGS)
        target_compile_options(${PROJECT_NAME} PUBLIC ${FLAG})
    endforeach()
endmacro()

macro(supress_cxx_compiler_warning WARNING)
    if (NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-${WARNING}")
    endif()
endmacro()
