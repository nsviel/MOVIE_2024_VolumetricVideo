#include "Namespace.h"

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API


int main(int argc, char * argv[]){
  //---------------------------

  //Init objects
  core::Opengl* opengl = new core::Opengl();
  realsense::Playback* realsense = new realsense::Playback(opengl);

  //Run program
  opengl->init();
  realsense->run();
  opengl->clean();

  //---------------------------
  return EXIT_SUCCESS;
}
