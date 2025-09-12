#include "rod.hpp"
Rod::Rod(Ball &mass1, Ball &mass2, float width_of_rod)
    : ball1(&mass1), ball2(&mass2), width(width_of_rod) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

}

void Rod::updatePosition() {
  glm::vec2 dir =
      glm::normalize(glm::vec2((*ball2).position.x - (*ball1).position.x,
                               (*ball2).position.y - (*ball1).position.y));
  glm::vec2 orthogonal = glm::vec2(-dir.y, dir.x) * (width * 0.5f);

  glm::vec2 v0 = glm::vec2((*ball1).position.x, (*ball1).position.y) +
                 orthogonal; // one corner
  glm::vec2 v1 = glm::vec2((*ball1).position.x, (*ball1).position.y) -
                 orthogonal; // opposite corner at p1
  glm::vec2 v2 = glm::vec2((*ball2).position.x, (*ball2).position.y) -
                 orthogonal; // opposite corner at p2
  glm::vec2 v3 = glm::vec2((*ball2).position.x, (*ball2).position.y) +
                 orthogonal; // last corner

  float vertices[] = {v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y};

  glBindVertexArray(VAO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void Rod::draw(Shader &shader) {
  shader.use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
