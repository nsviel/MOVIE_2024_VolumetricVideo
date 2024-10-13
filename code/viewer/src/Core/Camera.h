#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <GL/glu.h>


namespace core{

class Camera
{
public:
  //Constructor / Destructor
  Camera();
  ~Camera();

public:
  // Struct for Arcball camera state
  struct ArcballCamera {
      float angle_x = 0.0f;
      float angle_y = 0.0f;
      float distance = 2.5f;

      bool rotating = false;
      double last_x, last_y;
  };

  void set_projection_matrix(int width, int height);
  void apply_camera_transformations();

public:
  ArcballCamera camera;
};

}
