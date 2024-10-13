#include "Capture.h"

#include "GUI.h"
#include "../Core/Opengl.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace realsense{

//Constructor / Destructor
Capture::Capture(core::Opengl* opengl){
  //---------------------------

  this->opengl = opengl;
  this->gui = new realsense::GUI();

  //---------------------------
}
Capture::~Capture(){}

//Main function
void Capture::run(){
  //---------------------------

  // Declare pointcloud object, for calculating pointclouds and texture mappings
  rs2::pointcloud pc;
  rs2::points points;

  // Declare RealSense pipeline, encapsulating the actual device and sensors
  rs2::pipeline pipe;
  // Start streaming with default recommended configuration
  pipe.start();

  while (!glfwWindowShouldClose(opengl->window)){
    opengl->loop_start();

    gui->draw();

    // Wait for the next set of frames from the bag file
    auto frames = pipe.wait_for_frames();

    // Try to get a color frame
    auto color = frames.get_color_frame();

    // For cameras that don't have an RGB sensor, map to infrared instead
    if (!color)
        color = frames.get_infrared_frame();

    // Map pointcloud to the selected color frame
    pc.map_to(color);

    // Get the depth frame
    auto depth = frames.get_depth_frame();

    // Generate the pointcloud and texture mappings
    points = pc.calculate(depth);

    //opengl->draw_pointcloud(points, color);
    opengl->loop_end();

  }

  //---------------------------
}


}
