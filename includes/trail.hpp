#ifndef TRAIL_HPP
#define TRAIL_HPP

#include "ball.hpp"
#include "shader.hpp"
#include <deque>
#include <glm/glm.hpp>

struct Trail {
  unsigned int VAO, VBO;
  unsigned int length;
  std::deque<glm::vec2> trail_position;
  Ball *ball_to_trail;

  Trail(int length_of_trail, Ball &ball_to_be_trailed);
  void updateTrail();
  void draw(Shader &shader);
};

#endif
