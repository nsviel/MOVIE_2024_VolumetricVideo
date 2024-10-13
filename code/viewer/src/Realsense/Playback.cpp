#include "Playback.h"

#include "GUI.h"
#include "../Core/Opengl.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>


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

    this->draw_pointcloud(points, color);
    opengl->loop_end();
  }

  //---------------------------
}

//Subfunction
void Playback::draw_pointcloud(const rs2::points& points, const rs2::video_frame& color) {
  //---------------------------

  std::vector<glm::vec3> vec_xyz;
  std::vector<glm::vec3> vec_rgb;

  // Reserve memory to avoid reallocations
  vec_xyz.reserve(points.size());
  vec_rgb.reserve(points.size());

  auto vertices = points.get_vertices();
  auto tex_coords = points.get_texture_coordinates();

  int color_width = color.get_width();
  int color_height = color.get_height();
  const uint8_t* color_data = reinterpret_cast<const uint8_t*>(color.get_data());

  for (size_t i = 0; i < points.size(); ++i) {
    if (vertices[i].z) {
      // Map texture coordinates to pixel coordinates
      int u = glm::clamp(int(tex_coords[i].u * color_width), 0, color_width - 1);
      int v = glm::clamp(int(tex_coords[i].v * color_height), 0, color_height - 1);

      // Extract color data at the mapped pixel position
      int idx = (v * color_width + u) * 3; // RGB, so 3 channels per pixel
      glm::vec3 rgb = glm::vec3(color_data[idx], color_data[idx + 1], color_data[idx + 2]);

      // Set the point's position
      glm::vec3 xyz = glm::vec3(vertices[i].x, -vertices[i].y, vertices[i].z);

      // Add the point color and position to the vectors
      vec_rgb.push_back(rgb);
      vec_xyz.push_back(xyz);
    }
  }

  opengl->draw_pointcloud(vec_xyz, vec_rgb);

  //---------------------------
}

}
