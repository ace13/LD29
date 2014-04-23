cmake_minimum_required(VERSION 2.6)
project(LD29)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

if (MSVC)
    if (${MSVC_VERSION} VERSION_LESS 12.0)
        message(FATAL_ERROR "You need to compile this project with at least Visual Studio 2013")
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_UNICODE -DUNICODE)
elseif (UNIX)
    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (${GCC_VERSION} VERSION_GREATER 4.7 OR ${GCC_VERSION} VERSION_EQUAL 4.7)
            set(C11 "-std=c++11")
        elseif (${GCC_VERSION} VERSION_GREATER 4.3 OR ${GCC_VERSION} VERSION_EQUAL 4.3)
            set(C11 "-std=c++0x")
        else ()
            message(FATAL_ERROR "You need at least GCC 4.3 to compile this.")
        endif ()
    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(C11 "-std=c++11 -stdlib=libc++")
    endif ()

    set(CMAKE_CXX_FLAGS                "-Wall -Wextra ${C11} -pthread")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif ()

find_package(SFML REQUIRED COMPONENTS system window graphics)

include_directories(
    ${SFML_INCLUDE_DIR}
)

add_subdirectory(Source)