#pragma once

#include "ball.hpp"
#include "constant.hpp"
#include "shader.hpp"

struct Trail {
  void render();
  Trail(Ball &toTrack);
  void updateTrail();

private:
  Ball &tracked;
  std::deque<glm::vec3> trail;
  // float vertices[] = {
  //
  // };

  void setupVAO();
  unsigned int VAO, VBO;

  static Shader shader;
};
