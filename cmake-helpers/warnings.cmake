macro(add_cxx_compiler_flag FLAG)
    if (NOT CMAKE_CXX_FLAGS MATCHES ${FLAG})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
    endif()
endmacro()

# Set pedantic warnings on all platforms
# TODO: Remove code duplication
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        add_cxx_compiler_flag("/W4")
    endif()

    add_cxx_compiler_flag("/Wall")
    # Hardly usable on MSVC without excluding stdlib
    add_cxx_compiler_flag("/WX")
else()
     add_cxx_compiler_flag("-Wall")
     add_cxx_compiler_flag("-Wno-long-long")
     add_cxx_compiler_flag("-pedantic")
     add_cxx_compiler_flag("-Werror")
endif()

macro(supress_cxx_compiler_warning WARNING)
    if (MSVC)
    else()
        add_cxx_compiler_flag("-Wno-${WARNING}")
    endif()
endmacro()
