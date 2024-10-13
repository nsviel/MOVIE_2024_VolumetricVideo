#include "Playback.h"

#include "GUI.h"
#include "../Core/Opengl.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace realsense{

//Constructor / Destructor
Playback::Playback(core::Opengl* opengl){
  //---------------------------

  this->opengl = opengl;
  this->gui = new realsense::GUI();

  //---------------------------
}
Playback::~Playback(){}

//Main function
void Playback::run(){
  //---------------------------

  // Declare pointcloud object, for calculating pointclouds and texture mappings
  rs2::pointcloud pc;
  rs2::points points;
  rs2::pipeline pipe;

  // Set up configuration for playback from a bag file
  rs2::config cfg;
  const char* bag_file = "/home/nsv/Desktop/file.bag"; // Replace with your bag file path
  cfg.enable_device_from_file(bag_file); // Enable playback from the specified bag file

  // Start streaming with the configuration for the bag file
  pipe.start(cfg);

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

    opengl->draw_pointcloud(points, color);
    opengl->loop_end();

  }

  //---------------------------
}


}
