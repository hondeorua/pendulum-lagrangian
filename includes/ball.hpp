#pragma once

#include <glad/glad.h>
#include "constant.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

struct Ball {
  Ball();
  float vertices[resolution * 2 + 4];
  void render(Shader& shader);
private:
  void setupVAO();
  unsigned int VAO, VBO;
};
