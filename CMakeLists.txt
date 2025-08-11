cmake_minimum_required(VERSION 3.10)
project(gravitySim VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

link_directories(
    libs/glew/lib/Release/x64
)

# Include directories for GLFW and GLAD
include_directories(
    libs/glfw/include
    libs/glew/include
    
)

# Source files
set(SOURCE_FILES
    main.cpp
)

# Add executable
add_executable(gravitySim ${SOURCE_FILES})

# Link libraries
target_link_libraries(gravitySim
    glfw
    ${OPENGL_LIBRARIES}
    glew32 
)

# Build GLFW from source
add_subdirectory(libs/glfw)
