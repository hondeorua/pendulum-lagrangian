#include "ball.hpp"
// #include "container.hpp"

Ball::Ball(unsigned int resolution, float aspect_h_over_w, bool isPivot)
    : vertices(resolution * 2 + 4), isPivot(isPivot) {

  vertices[0] = 0.0;
  vertices[1] = 0.0;
  for (int i = 1; i < resolution + 1; i++) {
    float theta = 2 * M_PI * (i - 1) / resolution;
    vertices[i * 2] = std::cos(theta) * aspect_h_over_w;
    vertices[i * 2 + 1] = std::sin(theta);
  }
  vertices[resolution * 2 + 2] = aspect_h_over_w;
  vertices[resolution * 2 + 3] = 0.0;

  // velocity = glm::vec3(rando(-3.0f, 3.0f), rando(-3.0f, 3.0f), 0.0f);
  // position = glm::vec3(rando(Container::x, Container::x + Container::w)/2,
  // rando(Container::x, Container::x + Container::w)/2, 0.0f);
  // position = glm::vec3(rando(-1.0f, 1.0f) / 2, rando(-1.0f, 1.0f) / 2, 0.0f);
  // color = glm::vec3(rando(0.0f, 1.0f), rando(0.0f, 1.0f), rando(0.0f, 1.0f));
  color = glm::vec3(1.0f);
  aTrans = glm::mat4(1.0);
}

float Ball::rando(float min, float max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(min, max);

  return dist(gen);
}

