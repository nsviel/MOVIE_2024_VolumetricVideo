#pragma once

#include <GLFW/glfw3.h>

namespace core{class Camera;}


namespace core{

class Imgui
{
public:
  //Constructor / Destructor
  Imgui(core::Camera* camera);
  ~Imgui();

public:
  //Main function
  void init(GLFWwindow* window);
  void loop_start();
  void loop_end();
  void clean();

  //Subfunction
  void mouse_zoom();
  void mouse_arcball();

public:
  core::Camera* camera;
};

}
