cmake_minimum_required(VERSION 3.5)

project(
    embedded-qt-frontend
    VERSION 0.0.0
    LANGUAGES C CXX
)

option(BUILD_QT_FRONTEND "Build Qt frontend." ON)

if(BUILD_QT_FRONTEND)
    # Configure Qt.
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    # Create executable target.
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION)
    else()
        add_executable(${PROJECT_NAME})
    endif()
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    # Set warning level.
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4) # /WX
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
    endif()
    # Add additional include directories.
    target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    # Add sources.
    file(
        GLOB_RECURSE
        CXX_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
    )
    target_sources(${PROJECT_NAME} PRIVATE "${CXX_SOURCES}")
    # Link libraries.
    target_link_libraries(${PROJECT_NAME} PRIVATE embedded-user)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    # Set properties for Qt.
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER UnnamedOrange.experiment.com
                   MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                   MACOSX_BUNDLE_SHORT_VERSION_STRING
                   ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                   MACOSX_BUNDLE TRUE
                   WIN32_EXECUTABLE TRUE
    )
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${PROJECT_NAME})
    endif()
endif()
