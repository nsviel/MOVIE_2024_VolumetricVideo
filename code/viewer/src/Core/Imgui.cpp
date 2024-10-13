#include "Imgui.h"

#include "Camera.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace core{

//Constructor / Destructor
Imgui::Imgui(core::Camera* camera){
  //---------------------------

  this->camera = camera;

  //---------------------------
}
Imgui::~Imgui(){}

//Main function
void Imgui::init(GLFWwindow* window){
  //---------------------------

  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io; // You can add your fonts here if needed

  // Setup ImGui style
  ImGui::StyleColorsDark();

  // Initialize ImGui for GLFW and OpenGL
  ImGui_ImplGlfw_InitForOpenGL(window, true); // Make sure to pass your GLFW window
  ImGui_ImplOpenGL3_Init("#version 130"); // Adjust the GLSL version according to your needs

  //---------------------------
}
void Imgui::loop_start(){
  //---------------------------

  // Start the ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  this->mouse_zoom();
  this->mouse_arcball();

  //---------------------------
}
void Imgui::loop_end(){
  //---------------------------

  // Render the ImGui frame
  ImGui::Render();

  // Render ImGui
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  //---------------------------
}
void Imgui::clean(){
  //---------------------------


  //---------------------------
}

//Subfunction
void Imgui::mouse_zoom(){
  ImGuiIO& io = ImGui::GetIO();
  //---------------------------

  if (io.MouseWheel != 0) {
    camera->camera.distance -= io.MouseWheel * 0.05f;  // Adjust zoom speed
  }

  //---------------------------
}
void Imgui::mouse_arcball(){
  //---------------------------

  if (ImGui::IsMouseClicked(0)) { // 0 is the left mouse button
    camera->camera.rotating = true;
    ImVec2 mouse_pos = ImGui::GetMousePos();
    camera->camera.last_x = mouse_pos.x;
    camera->camera.last_y = mouse_pos.y;
  }
  else if (ImGui::IsMouseReleased(0)) {
    camera->camera.rotating = false;
  }

  ImVec2 mouse_pos = ImGui::GetMousePos();
  if (camera->camera.rotating){
    double dx = mouse_pos.x - camera->camera.last_x;
    double dy = mouse_pos.y - camera->camera.last_y;

    // Adjust these factors to reduce sensitivity
    const float sensitivity_x = 0.01f; // Lower value for less sensitivity
    const float sensitivity_y = 0.005f; // Lower value for less sensitivity

    camera->camera.angle_x += static_cast<float>(dx) * sensitivity_x;

    // Clamp the vertical angle to prevent the camera from flipping
    camera->camera.angle_y += static_cast<float>(dy) * sensitivity_y;
    camera->camera.angle_y = std::max(-1.5f, std::min(1.5f, camera->camera.angle_y)); // Clamp to about +/- 85 degrees

    camera->camera.last_x = mouse_pos.x;
    camera->camera.last_y = mouse_pos.y;
  }

  //---------------------------
}

}
