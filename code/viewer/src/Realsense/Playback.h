#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

namespace core{class Opengl;}
namespace realsense{class GUI;}


namespace realsense{

class Playback
{
public:
  //Constructor / Destructor
  Playback(core::Opengl* opengl);
  ~Playback();

public:
  //Main function
  void run();

public:
  core::Opengl* opengl;
  realsense::GUI* gui;
};

}
