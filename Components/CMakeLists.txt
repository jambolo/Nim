cmake_minimum_required(VERSION 3.21)
project(Components LANGUAGES CXX)

# Use modern CMake policies
cmake_policy(SET CMP0077 NEW)  # option() honors normal variables
cmake_policy(SET CMP0074 NEW)  # find_package uses <PackageName>_ROOT variables

#########################################################################
# Library Target                                                        #
#########################################################################

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        Board.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES
            Board.h
            Player.h
            Rules.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    DEBUG_POSTFIX d
    EXPORT_NAME ${PROJECT_NAME}
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            VC_EXTRALEAN
            _CRT_SECURE_NO_WARNINGS
            _SECURE_SCL=0
            _SCL_SECURE_NO_WARNINGS
    )
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(${PROJECT_NAME} 
#    PUBLIC
#)

# Organize source files for IDEs
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PRIVATE_SOURCES} ${PUBLIC_HEADERS})

#########################################################################
# Testing                                                               #
#########################################################################

# Only enable testing if it is explicitly requested. Project-wide testing is enabled in the root CMakeLists.txt.
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
