cmake_minimum_required(VERSION 3.5)

set(projectname "Uebung01")

#Ab hier normalerweise nichts zu ändern!
project(${projectname})

if(NOT DEFINED ${common_included})
    find_path(where_is_common NAMES "common.cmake" 
    PATHS "${CMAKE_SOURCE_DIR}" 
    "${CMAKE_SOURCE_DIR}/"
    "${CMAKE_SOURCE_DIR}/.."
    "${CMAKE_SOURCE_DIR}/../.."
    "${CMAKE_SOURCE_DIR}/../../../")
    include("${where_is_common}/common.cmake")
endif()
setup_project(${projectname})

if(WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${projectname} opengl32)
endif()
