cmake_minimum_required(VERSION 3.11)
set(PROJECT_NAME robot2DGame-template)


function(sfml_add_external)
    list(GET ARGN 0 target)
    list(REMOVE_AT ARGN 0)

    if (TARGET ${target})
        message(FATAL_ERROR "Target '${target}' is already defined")
    endif()

    cmake_parse_arguments(THIS "" "" "INCLUDE;LINK" ${ARGN})
    if (THIS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments when calling sfml_import_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    add_library(${target} INTERFACE)

    if (THIS_INCLUDE)
        foreach(include_dir IN LISTS THIS_INCLUDE)
            if (NOT include_dir)
                message(FATAL_ERROR "No path given for include dir ${THIS_INCLUDE}")
            endif()
            target_include_directories(${target} SYSTEM INTERFACE "$<BUILD_INTERFACE:${include_dir}>")
        endforeach()
    endif()

    if (THIS_LINK)
        foreach(link_item IN LISTS THIS_LINK)
            if (NOT link_item)
                message(FATAL_ERROR "Missing item in ${THIS_LINK}")
            endif()
            target_link_libraries(${target} INTERFACE "$<BUILD_INTERFACE:${link_item}>")
        endforeach()
    endif()

    install(TARGETS ${target} EXPORT SFMLConfigExport)
endfunction()

# Find the requested package and make an INTERFACE library from it
# The created INTERFACE library is tagged for export to be part of the generated SFMLConfig
# Usage: sfml_find_package(wanted_target_name
#                          [INCLUDE "OPENGL_INCLUDE_DIR"]
#                          [LINK "OPENGL_gl_LIBRARY"])
function(sfml_find_package)
    list(GET ARGN 0 target)
    list(REMOVE_AT ARGN 0)

    if (TARGET ${target})
        message(FATAL_ERROR "Target '${target}' is already defined")
    endif()

    cmake_parse_arguments(THIS "" "" "INCLUDE;LINK" ${ARGN})
    if (THIS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments when calling sfml_import_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    #set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")
    if (SFML_OS_IOS)
        find_host_package(${target} REQUIRED)
    else()
        find_package(${target} REQUIRED)
    endif()

    # Make sure to interpret the items in INCLUDE and LINK parameters. sfml_add_external()
    # does not interpret given items in order to also accept parameters that must not be interpreted
    set(LINK_LIST "")
    if (THIS_LINK)
        foreach(link_item IN LISTS THIS_LINK)
            list(APPEND LINK_LIST "${${link_item}}")
        endforeach()
    endif()

    set(INCLUDE_LIST "")
    if (THIS_INCLUDE)
        foreach(include_dir IN LISTS THIS_INCLUDE)
            list(APPEND INCLUDE_LIST "${${include_dir}}")
        endforeach()
    endif()

    sfml_add_external(${target} INCLUDE ${INCLUDE_LIST} LINK ${LINK_LIST})
endfunction()






if(MSVC)
    add_definitions(/MP)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory)
	and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_STANDARD 17)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

if(MSVC)
    enable_cxx_compiler_flag_if_supported("/W3")
    # Show C4068 (unknown pragma) only on warning level 4
    enable_cxx_compiler_flag_if_supported("/w44068")
    # Show C4200 (zero-sized array in struct/union) only on warning level 4
    enable_cxx_compiler_flag_if_supported("/w44200")
else()
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-Wpedantic")
    #enable_cxx_compiler_flag_if_supported("-Werror")
endif()

list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/AL")
list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/minimp3")
sfml_find_package(OpenAL INCLUDE "OPENAL_INCLUDE_DIR" LINK "OPENAL_LIBRARY")

include_directories(${CMAKE_SOURCE_DIR}/extlibs/minimp3)

set(LIBS)
if(UNIX)
    find_package(OpenGL REQUIRED)
    set(LIBS robot2D robot2D_ext OpenGL::GL ${CMAKE_DL_LIBS})
endif()

set(LIBS robot2DGame ${LIBS})
file(GLOB_RECURSE SRC source/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>")

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS} PRIVATE OpenAL)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res/
        ${CMAKE_CURRENT_BINARY_DIR}/res/)