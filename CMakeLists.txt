cmake_minimum_required(VERSION 3.0.0)
project(CGUebungen VERSION 0.1.0)

include(common.cmake)
set(common_included true)
add_subdirectory(SORS)

#Alle Unterverzeichnisse von CGR_Exercises
message("${PROJECT_SOURCE_DIR}/CGR_Exercises/*")
FILE(GLOB Uebungen "${PROJECT_SOURCE_DIR}/CGR_Exercises/*")
#Alle Unterverzeichnisse von ARTR_Exercises
message("${PROJECT_SOURCE_DIR}/ARTR_Exercises/*")
FILE(GLOB UebungenARTR "${PROJECT_SOURCE_DIR}/ARTR_Exercises/*")
#Alle Unterverzeichnisse von examples
message("${PROJECT_SOURCE_DIR}/examples/*")
FILE(GLOB ExamplesBasic "${PROJECT_SOURCE_DIR}/examples/*")
#Alle Unterverzeichnisse von vorlesungsbeispiele
message("${PROJECT_SOURCE_DIR}/vorlesungsbeispiele/*")
FILE(GLOB vorlesungsbeispiele "${PROJECT_SOURCE_DIR}/vorlesungsbeispiele/*")

# Alle Unterverzeichnisse in Uebungen und anderen hinzufügen
FOREACH(subdir ${Uebungen})
    IF(IS_DIRECTORY ${subdir})
        message("${subdir}")
        add_subdirectory(${subdir})
    ENDIF()
ENDFOREACH()

FOREACH(subdir ${UebungenARTR})
    IF(IS_DIRECTORY ${subdir})
        message("${subdir}")
        add_subdirectory(${subdir})
    ENDIF()
ENDFOREACH()

FOREACH(subdir ${ExamplesBasic})
    IF(IS_DIRECTORY ${subdir})
        message("${subdir}")
        add_subdirectory(${subdir})
    ENDIF()
ENDFOREACH()

FOREACH(subdir ${ExamplesAdvanced})
    IF(IS_DIRECTORY ${subdir})
        message("${subdir}")
        add_subdirectory(${subdir})
    ENDIF()
ENDFOREACH()
