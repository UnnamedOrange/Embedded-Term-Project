cmake_minimum_required(VERSION 3.5)

project(
    embedded-test
    VERSION 0.0.0
    LANGUAGES C CXX
)

option(BUILD_TEST "Build all tests.")

if(BUILD_TEST)
    # Introduce GoogleTest.
    add_subdirectory("../dep/googletest" "dep/googletest")
    include(GoogleTest)

    # Enable testing.
    enable_testing()

    # Create test target.
    add_executable(${PROJECT_NAME})
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    # Set warning level.
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4) # /WX
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
    endif()
    # Add additional include directories.
    target_include_directories(
        ${PROJECT_NAME} PRIVATE "${gtest_SOURCE_DIR}" "${gtest_SOURCE_DIR}/include"
    )
    # Add sources.
    file(GLOB_RECURSE CXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    target_sources(${PROJECT_NAME} PRIVATE "${CXX_SOURCES}")
    # Link libraries.
    target_link_libraries(${PROJECT_NAME} PRIVATE embedded-user)
    target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)

    gtest_discover_tests(${PROJECT_NAME})
endif()
