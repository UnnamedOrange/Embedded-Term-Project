cmake_minimum_required(VERSION 3.5)

project(
    embedded-user
    VERSION 0.0.0
    LANGUAGES C CXX
)

option(BUILD_I2C "Enable I2C data source." ON)

# Create library target.
add_library(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Add customed definitions.
if(BUILD_I2C)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_I2C=1)
endif()
# Set warning level.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4) # /WX
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
# Add additional include directories.
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dep/readerwriterqueue")
# Add sources.
file(GLOB_RECURSE CXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
target_sources(${PROJECT_NAME} PRIVATE "${CXX_SOURCES}")
# Link libraries.
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread stdc++fs)
endif()
