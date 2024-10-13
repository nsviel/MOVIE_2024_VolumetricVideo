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


  glfwInit();
  glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // Create a simple OpenGL window for rendering
  this->window = this->init_window(1280, 720, "Volumetric video");

  imgui->init(window);

  // Initialize GL settings
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0f);

  // Set callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  camera->set_projection_matrix(1280, 720);

  glewInit();

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
void Opengl::draw_pointcloud(std::vector<glm::vec3>& vec_xyz, std::vector<glm::vec3>& vec_rgb){
  //---------------------------

  static GLuint vao;
  static GLuint vbo_vertices;
  static GLuint vbo_colors;
  static bool buffers_initialized = false;
  if (!buffers_initialized) {
      // Generate and bind VAO
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      // Generate and bind VBO for vertices
      glGenBuffers(1, &vbo_vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
      glBufferData(GL_ARRAY_BUFFER, vec_xyz.size() * sizeof(glm::vec3), vec_xyz.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(0);

      // Generate and bind VBO for colors
      glGenBuffers(1, &vbo_colors);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
      glBufferData(GL_ARRAY_BUFFER, vec_rgb.size() * sizeof(glm::vec3), vec_rgb.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
      glEnableVertexAttribArray(1);

      // Unbind VAO
      glBindVertexArray(0);

      buffers_initialized = true;
  } else {
      // Update VBO data if points change
      glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vec_xyz.size() * sizeof(glm::vec3), vec_xyz.data());

      glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vec_rgb.size() * sizeof(glm::vec3), vec_rgb.data());
  }

  // Render the point cloud
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, vec_xyz.size());
  glBindVertexArray(0);

/*
  glBegin(GL_POINTS);
  for (size_t i = 0; i < vec_xyz.size(); ++i) {
    // Set the color for the point
    glColor3ub(vec_rgb[i].x, vec_rgb[i].y, vec_rgb[i].z);

    // Set the position for the point
    glVertex3f(vec_xyz[i].x, vec_xyz[i].y, vec_xyz[i].z);
  }
  glEnd();
*/
  //---------------------------
}

}
