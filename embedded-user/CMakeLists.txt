cmake_minimum_required(VERSION 3.5)

project(
    embedded-user
    VERSION 0.0.0
    LANGUAGES C CXX
)

# Create executable target.
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Set warning level.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4) # /WX
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
# Add additional include directories.
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
# Add sources.
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
