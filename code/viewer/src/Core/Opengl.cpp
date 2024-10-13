#include "Opengl.h"

#include "Camera.h"
#include "Imgui.h"

//Callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  //---------------------------

  glViewport(0, 0, width, height); // Update the viewport to the new window dimensions
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the field of view, aspect ratio, and near/far planes
  gluPerspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 50.0f); // Near plane at 0.1, far plane at 50.0

  glMatrixMode(GL_MODELVIEW); // Return to model view matrix mode

  //---------------------------
}

namespace core{

//Constructor / Destructor
Opengl::Opengl(){
  //---------------------------

  this->camera = new core::Camera();
  this->imgui = new core::Imgui(camera);

  //---------------------------
}
Opengl::~Opengl(){}

//Main function
void Opengl::init(){
  //---------------------------

  // Create a simple OpenGL window for rendering
  this->window = this->init_window(1280, 720, "Volumetric video");

  imgui->init(window);

  // Initialize GL settings
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0f);

  // Set callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  camera->set_projection_matrix(1280, 720);

  //---------------------------
}
void Opengl::loop_start(){
  //---------------------------

  // Clear the current buffer before anything else
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers first

  glLoadIdentity(); // Load identity matrix
  camera->apply_camera_transformations(); // Apply camera transformations

  imgui->loop_start();

  //---------------------------
}
void Opengl::loop_end(){
  //---------------------------

  this->draw_axes(); // Draw axes (if applicable)
  imgui->loop_end(); // Render ImGui frame

  // Swap front and back buffers
  glfwSwapBuffers(window); // Swap buffers
  glfwPollEvents(); // Poll for events

  //---------------------------
}
void Opengl::clean(){
  //---------------------------

  glfwDestroyWindow(window);
  glfwTerminate();

  //---------------------------
}

//Subfunction
GLFWwindow* Opengl::init_window(int width, int height, const char* title){
  //---------------------------

  if (!glfwInit())
      throw std::runtime_error("Failed to initialize GLFW");

  GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
  {
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window);

  //---------------------------
  return window;
}
void Opengl::draw_axes(){
  //---------------------------

  glLineWidth(3.0f);
  glBegin(GL_LINES);

  // Draw X axis (Red)
  glColor3f(1.0f, 0.0f, 0.0f); // Red color
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f); // Length of 1 unit

  // Draw Y axis (Green)
  glColor3f(0.0f, 1.0f, 0.0f); // Green color
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f); // Length of 1 unit

  // Draw Z axis (Blue)
  glColor3f(0.0f, 0.0f, 1.0f); // Blue color
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 1.0f); // Length of 1 unit

  glEnd();
  glLineWidth(1.0f);

  //---------------------------
}
void Opengl::draw_pointcloud(const rs2::points& points, const rs2::video_frame& color){
  //---------------------------

  glBegin(GL_POINTS);
  auto vertices = points.get_vertices();
  auto tex_coords = points.get_texture_coordinates();

  int color_width = color.get_width();
  int color_height = color.get_height();
  const uint8_t* color_data = reinterpret_cast<const uint8_t*>(color.get_data());

  for (size_t i = 0; i < points.size(); ++i)
  {
      if (vertices[i].z) // Skip zero depth points
      {
          // Map texture coordinates to pixel coordinates
          int u = std::min(std::max(int(tex_coords[i].u * color_width), 0), color_width - 1);
          int v = std::min(std::max(int(tex_coords[i].v * color_height), 0), color_height - 1);

          // Extract color data at the mapped pixel position
          int idx = (v * color_width + u) * 3; // RGB, so 3 channels per pixel
          uint8_t r = color_data[idx];
          uint8_t g = color_data[idx + 1];
          uint8_t b = color_data[idx + 2];

          //location
          float x = vertices[i].x * 1;
          float y = -vertices[i].y * 1;
          float z = vertices[i].z * 1;

          // Set the point color and position
          glColor3ub(r, g, b);
          glVertex3f(x ,y, z);
      }
  }
  glEnd();

  //---------------------------
}

}
