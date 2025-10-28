#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ball.hpp"
#include "shader.hpp"

struct Rod {
  void render();
  Rod(Ball &ball1, Ball &ball2);
private:
  float vertices[12] = {ROD_WIDTH,  0.0,         -ROD_WIDTH, 0.0,
                        -ROD_WIDTH, -ROD_LENGTH, -ROD_WIDTH, -ROD_LENGTH,
                        ROD_WIDTH,  0.0,         ROD_WIDTH,  -ROD_LENGTH};
  glm::vec3 position = glm::vec3(0.0f);
  static Shader shader;
  unsigned int VAO, VBO;
  Ball &ball1;
  Ball &ball2;

  void setupVAO();
  void updatePosition();
  float findAngle();
};
