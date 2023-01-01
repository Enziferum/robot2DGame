function(robot2D_add_external)
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

    install(TARGETS ${target} EXPORT Robot2DConfigExport)
endfunction()

# Find the requested package and make an INTERFACE library from it
# The created INTERFACE library is tagged for export to be part of the generated Robot2DConfig
# Usage: robot2D_find_package(wanted_target_name
#                          [INCLUDE "OPENGL_INCLUDE_DIR"]
#                          [LINK "OPENGL_gl_LIBRARY"])
function(robot2D_find_package)
    list(GET ARGN 0 target)
    list(REMOVE_AT ARGN 0)

    if (TARGET ${target})
        message(FATAL_ERROR "Target '${target}' is already defined")
    endif()

    cmake_parse_arguments(THIS "" "" "INCLUDE;LINK" ${ARGN})
    if (THIS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments when calling robot2D_import_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    #set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")
    if (ROBOT2D_OS_IOS)
        find_host_package(${target} REQUIRED)
    else()
        find_package(${target} REQUIRED)
    endif()

    # Make sure to interpret the items in INCLUDE and LINK parameters. robot2D_add_external()
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

    robot2D_add_external(${target} INCLUDE ${INCLUDE_LIST} LINK ${LINK_LIST})
endfunction()