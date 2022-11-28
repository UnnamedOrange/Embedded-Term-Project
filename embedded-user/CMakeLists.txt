cmake_minimum_required(VERSION 3.5)

project(
    embedded
    VERSION 0.0.0
    LANGUAGES C CXX
)

# Add the core library.
add_subdirectory("src/user")
# Add Qt frontend executable (optional).
add_subdirectory("src/qt-frontend")

# Create executable target.
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Set warning level.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4) # /WX
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) # -Werror
endif()
# Add sources.
target_sources(${PROJECT_NAME} PRIVATE "src/main.cpp")
# Link libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE embedded-user)

# Add tests.
add_subdirectory("test")
