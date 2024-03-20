cmake_minimum_required(VERSION 3.11)
project("SORSLib")

set(CMAKE_PREFIX_PATH  "C:/Qt/6.2.0/msvc2019_64/lib/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOGEN_PARALLEL 12)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)

FILE(GLOB header_project RELATIVE ${PROJECT_SOURCE_DIR} *.hpp */*.hpp)
LIST(FILTER header_project EXCLUDE REGEX ".*SORSLib.*")
MESSAGE("DBG Headers are ${header_project}")

FILE(GLOB src_project RELATIVE ${PROJECT_SOURCE_DIR} *.cpp */*.cpp)
LIST(FILTER header_project EXCLUDE REGEX ".*SORSLib.*")

file(GLOB shader_project RELATIVE ${PROJECT_SOURCE_DIR} "shader/*")
message("shader_project: ${shader_project}")

file(GLOB ui_project "*${PROJECT_SOURCE_DIR}/.ui")
file(GLOB qrc_project "${PROJECT_SOURCE_DIR}/*.qrc")
qt_add_resources(qrcfiles ${qrc_project})

message("CMAKE: ${CMAKE_SOURCE_DIR}")
message("CMAKE: ${PROJECT_SOURCE_DIR}")

#Zielordner finden und setzen, CMAKE Ordner ausschlie�en, sonst findet man unter Windows im build-Dir
find_path(where_is_lib NAMES "SORSLib"
PATHS 
"${CMAKE_SOURCE_DIR}/../../../SORS/"
"${CMAKE_SOURCE_DIR}/../../SORS/"
"${CMAKE_SOURCE_DIR}/../SORS/"
"${CMAKE_SOURCE_DIR}/SORS/"
NO_CACHE
REQUIRED
NO_DEFAULT_PATH
NO_PACKAGE_ROOT_PATH
NO_CMAKE_PATH
NO_CMAKE_ENVIRONMENT_PATH
NO_SYSTEM_ENVIRONMENT_PATH
NO_CMAKE_SYSTEM_PATH)

message("Found Lib path in ${where_is_lib}/SORSLib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${where_is_lib}/SORSLib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${where_is_lib}/SORSLib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${where_is_lib}/SORSLib")

add_library(SORSLib ${header_project} ${src_project} ${uifiles} ${qrcfiles} ${shader_project})

#include_directories("${where_is_lib}/SORSLib")
target_include_directories(SORSLib PUBLIC ${PROJECT_SOURCE_DIR} PUBLIC ${where_is_lib}SORSLib/)

target_link_libraries(SORSLib Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets)
target_compile_definitions(SORSLib PUBLIC DEFAULTSHADERDIR="${where_is_lib}shader")

#message("Copy headers to lib directory ... ${header_project} to ${where_is_lib}SORSLib/")
#add_custom_command(
#        TARGET SORSLib  PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
#                ${header_project}
#                ${where_is_lib}SORSLib/)
