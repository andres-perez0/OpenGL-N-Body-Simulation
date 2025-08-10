cmake_minimum_required(VERSION 3.10)
project(YourOpenGLProject VERSION 1.0)

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
    # libs/glad/src/glad.c
)

# Add executable
add_executable(YourOpenGLProject ${SOURCE_FILES})

# Link libraries
target_link_libraries(YourOpenGLProject
    glfw
    ${OPENGL_LIBRARIES}
    glew32 
)

# Build GLFW from source
add_subdirectory(libs/glfw)

# cmake_minimum_required(VERSION 3.0.0)
# # specifiy min version
# project(MyCMakeproject VERSION 0.1.0 LANGUAGES C CXX)
# # project name | languages
# include (CTest)
# # testing tool with cmake
# enable_testing()

# add_executable(MyCMakeproject mycppcode.cpp)
# # The type of execute that you want to generate for the project
# set_property(TARGET MyCMakeproject PROPERTY CXX_STANDARD 17)
# # Gives you the standard of code so you can the appropiate intelligence
