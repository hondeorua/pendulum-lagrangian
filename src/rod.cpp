#include "rod.hpp"
#include "constant.hpp"
#include "glm/ext/matrix_transform.hpp"

Shader Rod::shader;

Rod::Rod(Ball &ball1, Ball &ball2) : ball1(ball1), ball2(ball2) {
  setupVAO();
  shader.makeShader("../src/shader/rod.vert", "../src/shader/rod.frag");
  shader.use();
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
}
void Rod::render() {
  updatePosition();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, findAngle(), glm::vec3(0, 0, 1));

  shader.use();
  shader.setMat4("model", model);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Rod::setupVAO() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
}

void Rod::updatePosition() {
  position = ball1.getPosition();
}

float Rod::findAngle() {
  glm::vec3 vect = ball2.getPosition() - ball1.getPosition();
  return atan2(vect.x, -vect.y);
}
