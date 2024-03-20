#CMAKE den Pfad zu Qt mitteilen
#Der Pfad zu cmake muss unter Windows noch in PATH geschrieben werden
#also z.B.: C:/Qt/6.4.0/msvc2019_64/lib/cmake
if(WIN32)
    set(CMAKE_PREFIX_PATH  "C:/Qt/6.5.1/msvc2019_64/lib/cmake")
else()
    set(CMAKE_PREFIX_PATH  "/home/benne/Qt/6.6.0/gcc_64/lib/cmake")
endif()
message("Looking for Qt in ${CMAKE_PREFIX_PATH}")

# Qt automatische Generierung aus qrc, ui, etc.
# Muss außerhalb der Funktion unten sein (warum auch immer)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC OFF)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)

function(setup_project projectname)

    project(${projectname})

    #Library-Ordner finden und setzen, CMAKE Ordner ausschließen, sonst findet man unter Windows im build-Dir
    find_path(SORS_IncludePath NAMES "SORSLib"
    PATHS 
    "${CMAKE_SOURCE_DIR}/../../../SORS/"
    "${CMAKE_SOURCE_DIR}/../../SORS/"
    "${CMAKE_SOURCE_DIR}/../SORS/"
    "${CMAKE_SOURCE_DIR}/SORS/"
    "/home/benne/vorlesungen/CGRelaunched/SORS_Code/SORS"
    NO_CACHE
    REQUIRED
    NO_DEFAULT_PATH
    NO_PACKAGE_ROOT_PATH
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_SYSTEM_PATH)
    set(SORS_Include ${SORS_IncludePath}SORSLib)
    message("Found headers in ${SORS_Include}")

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    file(GLOB_RECURSE header_project "${PROJECT_SOURCE_DIR}/*.h")
    list(FILTER header_project EXCLUDE REGEX ".*/build.*")
    file(GLOB_RECURSE headerpp_project "${PROJECT_SOURCE_DIR}/*.hpp")
    list(FILTER headerpp_project EXCLUDE REGEX ".*/build.*")
    file(GLOB_RECURSE src_project "${PROJECT_SOURCE_DIR}/*.cpp")
    list(FILTER src_project EXCLUDE REGEX ".*/build.*")
    file(GLOB_RECURSE ui_project "${PROJECT_SOURCE_DIR}/*.ui")
    list(FILTER ui_project EXCLUDE REGEX ".*/build.*")
    file(GLOB_RECURSE qrc_project "${PROJECT_SOURCE_DIR}/*.qrc")
    list(FILTER qrc_project EXCLUDE REGEX ".*/build.*")
    qt_add_resources(qrcfiles ${qrc_project})
    #qt_wrap_ui(uifiles ${ui_project})

    # Shader Dateien grundsätzlich aus Unterverzeichnis "shader" hinzufügen
        file(GLOB_RECURSE shader_project "${PROJECT_SOURCE_DIR}/shader/*.*")
        list(FILTER shader_project EXCLUDE REGEX ".*/build.*")
    # Shader Dateien grundsätzlich aus Unterverzeichnis "shader_vorlagen" für Übungsvorlagen hinzufügen
        file(GLOB_RECURSE shader_vorlagen_project "${PROJECT_SOURCE_DIR}/shader_vorlagen/*.*")
        list(FILTER shader_project EXCLUDE REGEX ".*/build.*")
    # Dateien wie Bilder, Modelle, etc. grundsätzlich aus Unterverzeichnis "shader" hinzufügen
        file(GLOB_RECURSE data_project "${PROJECT_SOURCE_DIR}/data/*.*")
        list(FILTER src_project EXCLUDE REGEX ".*/build.*")


    #Build und Source Dir inkludieren, um ggf. generierte Header aus ui-Files zu finden
    message("Include ${CMAKE_CURRENT_BINARY_DIR} hinzufuegen")
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    message("Include ${PROJECT_SOURCE_DIR} hinzufuegen")
    include_directories(${PROJECT_SOURCE_DIR})
    
    include_directories(${SORS_Include})

    if(WIN32)
        if (NOT TARGET SORSLib)
            add_library(SORSLib STATIC IMPORTED)
        endif()
        set_target_properties(SORSLib PROPERTIES IMPORTED_LOCATION_DEBUG "${SORS_Include}/Debug/SORSLib.lib")
        set_target_properties(SORSLib PROPERTIES IMPORTED_LOCATION_RELEASE "${SORS_Include}/Release/SORSLib.lib")
    endif()
    link_directories(${SORS_Include})
    link_directories(${where_are_qt_dlls})
    add_executable(${projectname} ${src_project} ${headerpp_project}  ${header_project}
        ${uifiles} ${qrcfiles} ${ui_project} ${shader_project} ${shader_vorlagen_project} ${data_project})
    target_link_libraries(${projectname} SORSLib)
    target_link_libraries(${projectname} Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets)
    target_compile_definitions(${projectname} PUBLIC SRCDIR="${PROJECT_SOURCE_DIR}/")
    target_compile_definitions(${projectname} PUBLIC SORSDIR="${SORS_Include}/../")
    target_compile_definitions(${projectname} PUBLIC DEFAULT_IMAGES_DIR="${SORS_Include}/../../images/")

endfunction()
