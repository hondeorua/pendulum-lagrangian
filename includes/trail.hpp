#ifndef TRAIL_HPP
#define TRAIL_HPP

#include "ball.hpp"
#include "shader.hpp"
#include <deque>
#include <glm/glm.hpp>

struct Trail {
  unsigned int VAO, VBO;
  unsigned int length;
  std::deque<glm::vec3> trail_position;
  Ball ball_of_trail;
  Ball *ball_to_trail;
  float scale;
  int resolution;

  Trail(int length_of_trail, int resolution, float aspect_h_over_w,
        float SCALE, Ball& ball_to_be_trailed);
  void updateTrail();
  void draw(Shader &shader);
};

#endif
