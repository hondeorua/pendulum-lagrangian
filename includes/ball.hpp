#ifndef BALL_HPP
#define BALL_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <vector>

class Ball {

public:
  std::vector<float> vertices;

  bool isPivot;
  float mass;
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 color;
  glm::mat4 aTrans;

  Ball(unsigned int resolution, float aspect_h_over_w, float mass_input = 1, bool isPivot = false);

private:
  float rando(float min, float max);
};

#endif
