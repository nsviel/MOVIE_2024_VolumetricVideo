#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <glm/glm.hpp>

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

  //Subfunction
  void draw_pointcloud(const rs2::points& points, const rs2::video_frame& color);
  void display_image_color(const rs2::video_frame& color);
  void display_image_depth(const rs2::video_frame& depth);
  
public:
  core::Opengl* opengl;
  realsense::GUI* gui;
};

}
