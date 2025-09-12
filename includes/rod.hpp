#ifndef ROD_HPP
#define ROD_HPP

#include "ball.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>

struct Rod {
  Ball *ball1;
  Ball *ball2;

  float width;

  unsigned int VAO, VBO;

  Rod(Ball &mass1, Ball &mass2, float width_of_rod = 0.01f);

  void updatePosition();

  void draw(Shader &shader);
};

#endif
