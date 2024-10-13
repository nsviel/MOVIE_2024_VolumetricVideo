set(LIBRARY_NAME MAIN_EXTERN)

#-----------------
# Source files
#-----------------
file(GLOB_RECURSE ALL_SOURCES
  *.cpp
  *.h
)

#-----------------
# Compilation
#-----------------
add_library(${LIBRARY_NAME}
  ${ALL_SOURCES}
)

#-----------------
# Dependencies
#-----------------
target_include_directories(${LIBRARY_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
