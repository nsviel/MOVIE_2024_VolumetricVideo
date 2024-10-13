set(EXECUTABLE_NAME executable)

#-----------------
# Source files
#-----------------
file(GLOB_RECURSE ALL_SOURCES
  *.h
  *.cpp
  main.cpp
)


#-----------------
# Dependencies
#-----------------
# Find required packages
find_package(realsense2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Include directories
include_directories(
  ${realsense2_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIRS}
  ${IMGUI_DIR} # Include ImGui directory for headers
)

#-----------------
# Compilation
#-----------------
add_executable(${EXECUTABLE_NAME}
  ${ALL_SOURCES}
  main.cpp
)

#-----------------
# Dependencies
#-----------------
target_link_libraries(${EXECUTABLE_NAME}
  MAIN_EXTERN

  ${realsense2_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}

  -lGLEW
)
