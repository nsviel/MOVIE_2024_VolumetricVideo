#-----------------
# Project
#-----------------
cmake_minimum_required(VERSION 3.5)
project(RealSensePointCloudViewer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(OpenGL_GL_PREFERENCE GLVND)

#-----------------
# Source files
#-----------------
add_subdirectory(extern)
add_subdirectory(src)
