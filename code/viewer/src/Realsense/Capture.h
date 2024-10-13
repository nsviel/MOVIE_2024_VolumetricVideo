#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

namespace core{class Opengl;}
namespace realsense{class GUI;}


namespace realsense{

class Capture
{
public:
  //Constructor / Destructor
  Capture(core::Opengl* opengl);
  ~Capture();

public:
  //Main function
  void run();

public:
  core::Opengl* opengl;
  realsense::GUI* gui;
};

}
