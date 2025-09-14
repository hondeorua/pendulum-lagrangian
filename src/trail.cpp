#include "trail.hpp"
#include "shader.hpp"

Trail::Trail(int length_of_trail, Ball &ball_to_be_trailed)
    : length(length_of_trail), ball_to_trail(&ball_to_be_trailed) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * length, nullptr,
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
}

void Trail::updateTrail() {
  trail_position.push_back((*ball_to_trail).position);
  if (trail_position.size() > length)
    trail_position.pop_front();
}

void Trail::draw(Shader &shader) {
  shader.use();
  shader.setInt("trailLength", length);

  std::vector<float> vertices;
  for (glm::vec2 n : trail_position) {
    vertices.push_back(n.x);
    vertices.push_back(n.y);
  }

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
                  vertices.data());
  glDrawArrays(GL_LINE_STRIP, 0, trail_position.size());
}
