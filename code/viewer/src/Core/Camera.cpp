#include "Camera.h"


namespace core{

//Constructor / Destructor
Camera::Camera(){
  //---------------------------



  //---------------------------
}
Camera::~Camera(){}

//Main function
void Camera::set_projection_matrix(int width, int height){
  //---------------------------

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the field of view, aspect ratio, and near/far planes
  gluPerspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 50.0f); // Near plane at 0.1, far plane at 50.0

  glMatrixMode(GL_MODELVIEW); // Return to model view matrix mode

  //---------------------------
}
void Camera::apply_camera_transformations(){
  //---------------------------

  glTranslatef(0.0f, 0.0f, -camera.distance);
  glRotatef(camera.angle_y * 180.0f / M_PI, 1.0f, 0.0f, 0.0f);
  glRotatef(camera.angle_x * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);

  //---------------------------
}

}
