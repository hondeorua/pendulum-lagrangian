#pragma once

#include "const.hpp"
#include <GLFW/glfw3.h>

struct Ball {
  Ball();
  float vertices[resolution * 2 + 1];
};
