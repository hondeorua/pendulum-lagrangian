#pragma once

#include <glad/glad.h>
#include "constant.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

struct Ball {
  Ball();
  void render();
  void updatePosition(const glm::vec3);
  const glm::vec3& getPosition() const;
private:
  void setupVAO();
  unsigned int VAO, VBO;
  float vertices[resolution * 2 + 4];

  glm::vec3 position = glm::vec3(0.0f);

  static Shader shader;
};
