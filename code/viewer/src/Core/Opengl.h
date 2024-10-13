#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <GL/glu.h>

namespace core{class Camera;}
namespace core{class Imgui;}


namespace core{

class Opengl
{
public:
  //Constructor / Destructor
  Opengl();
  ~Opengl();

public:
  //Main function
  void init();
  void loop_start();
  void loop_end();
  void clean();

  //Subfunction
  GLFWwindow* init_window(int width, int height, const char* title);
  void draw_axes();
  void draw_pointcloud(std::vector<glm::vec3>& vec_xyz, std::vector<glm::vec3>& vec_rgb);

public:
  core::Camera* camera;
  core::Imgui* imgui;

  GLFWwindow* window;
};

}
