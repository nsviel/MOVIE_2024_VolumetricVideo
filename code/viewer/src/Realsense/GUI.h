#pragma once


namespace realsense{

class GUI
{
public:
  //Constructor / Destructor
  GUI();
  ~GUI();

public:
  //Main function
  void draw();

public:
  bool window_open = true;
};

}
