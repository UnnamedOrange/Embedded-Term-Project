cmake_minimum_required(VERSION 3.5)

project(
    embedded-user
    VERSION 0.0.0
    LANGUAGES C CXX
)

option(BUILD_I2C "Enable I2C data source." ON)

# Setup libhv.
set(BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE
)
set(BUILD_STATIC
    OFF
    CACHE BOOL "" FORCE
)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/libhv" "${CMAKE_BINARY_DIR}/dep/libhv")
set(HV_SRCDIRS
    .
    base
    ssl
    event
    event/kcp
    util
    cpputil
    evpp
    protocol
    http
    http/client
    http/server
    mqtt
)
foreach(SRCDIR ${HV_SRCDIRS})
    list(APPEND HV_SRCDIRS_PREFIXED "${CMAKE_SOURCE_DIR}/dep/libhv/${SRCDIR}")
endforeach()

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
target_include_directories(${PROJECT_NAME} PUBLIC "${HV_SRCDIRS_PREFIXED}")
if(BUILD_I2C)
    target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/dep/bcm2835")
endif()
# Add sources.
file(GLOB_RECURSE CXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
target_sources(${PROJECT_NAME} PRIVATE "${CXX_SOURCES}")
# Link libraries.
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread stdc++fs atomic)
endif()
if(BUILD_I2C)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/dep/bcm2835/bcm2835.c")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE hv)
