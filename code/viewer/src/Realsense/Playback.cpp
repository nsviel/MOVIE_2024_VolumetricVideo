#include "Playback.h"

#include "GUI.h"
#include "../Core/Opengl.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <opencv2/opencv.hpp>
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
    this->display_image_color(color);

    // For cameras that don't have an RGB sensor, map to infrared instead
    if (!color)
      color = frames.get_infrared_frame();

    // Map pointcloud to the selected color frame
    pc.map_to(color);

    // Get the depth frame
    auto depth = frames.get_depth_frame();
    this->display_image_depth(depth);

    // Generate the pointcloud and texture mappings
    points = pc.calculate(depth);

    this->draw_pointcloud(points, color);
    opengl->loop_end();
  }

  //---------------------------
}

//Subfunction
void Playback::draw_pointcloud(const rs2::points& points, const rs2::video_frame& color){
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
void Playback::display_image_color(const rs2::video_frame& color){
  //---------------------------

  if (color) {
    // Retrieve color frame dimensions
    int width = color.get_width();
    int height = color.get_height();

    // Convert the color frame to OpenCV Mat
    cv::Mat color_image(cv::Size(width, height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);

    // Convert from RGB to BGR as OpenCV uses BGR format by default
  //  cv::cvtColor(color_image, color_image, cv::COLOR_RGB2BGR);

    // Resize the image to 150x150 pixels
    int new_width = 500;
    int new_height = static_cast<int>(height * (static_cast<float>(new_width) / width));
    cv::Mat resized_image;
    cv::resize(color_image, resized_image, cv::Size(new_width, new_height));

    // Display the resized image using OpenCV
    cv::imshow("Color Frame", resized_image);
    cv::waitKey(1);
  }

  //---------------------------
}
void Playback::display_image_depth(const rs2::video_frame& depth) {
  //---------------------------

  if (depth) {
    // Retrieve depth frame dimensions
    int width = depth.get_width();
    int height = depth.get_height();

    // Convert the depth frame to OpenCV Mat
    cv::Mat depth_image(cv::Size(width, height), CV_16UC1, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

    // Normalize the depth values to the range [0, 255] for visualization
    cv::Mat depth_normalized;
    depth_image.convertTo(depth_normalized, CV_8UC1, 255.0 / 5000.0); // Assuming depth values range from 0 to 65535

    // Apply a colormap to the normalized depth image
    cv::Mat depth_colormap;
    cv::applyColorMap(depth_normalized, depth_colormap, cv::COLORMAP_JET); // You can choose any colormap

    // Set the new width and calculate height to maintain aspect ratio
    int new_width = 500;  // You can adjust this value
    int new_height = static_cast<int>(height * (static_cast<float>(new_width) / width));

    // Resize the colormap image while maintaining aspect ratio
    cv::Mat resized_depth_colormap;
    cv::resize(depth_colormap, resized_depth_colormap, cv::Size(new_width, new_height));

    // Display the resized depth colormap image using OpenCV
    cv::imshow("Depth Colormap", resized_depth_colormap);
    cv::waitKey(1);
  }

  //---------------------------
}

}
