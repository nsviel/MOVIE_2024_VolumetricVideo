#include "GUI.h"

#include <imgui/imgui.h>


namespace realsense{

//Constructor / Destructor
GUI::GUI(){
  //---------------------------


  //---------------------------
}
GUI::~GUI(){}

//Main function
void GUI::draw(){
  //---------------------------

  ImGui::Begin("My Window"); // Create a new ImGui window
  ImGui::Text("Hello, world!"); // Display some text
  ImGui::End(); // End the ImGui window

  //---------------------------
}


}
