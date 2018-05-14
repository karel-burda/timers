# Set pedantic warnings on all platforms
# TODO: Remove code duplication
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    if (NOT CMAKE_CXX_FLAGS MATCHES "/Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    endif()

    if (NOT CMAKE_CXX_FLAGS MATCHES "/WX")
        # Hardly usable on MSVC without excluding stdlib
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    endif()
else()
    if (NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    endif()

    if (NOT CMAKE_CXX_FLAGS MATCHES "-Wno-long-long")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
    endif()

    if (NOT CMAKE_CXX_FLAGS MATCHES "-pedantic")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    endif()

    if (NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif()
endif()
