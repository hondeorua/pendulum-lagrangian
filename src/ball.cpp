#include "ball.hpp"
#include "constant.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"

// initialize static variable
Shader Ball::shader;

Ball::Ball() {
  // Center
  vertices[0] = 0.0;
  vertices[1] = 0.0;

  for (unsigned int i = 1; i <= resolution; ++i) {
    double theta = 2 * PI * i / resolution;
    vertices[2 * i] = std::cos(theta);
    vertices[2 * i + 1] = std::sin(theta);
  }

  // Add needed point to complete triagle
  vertices[2 * resolution + 2] = vertices[2];
  vertices[2 * resolution + 3] = vertices[3];
  setupVAO();

  shader.makeShader("../src/shader/ball.vert", "../src/shader/ball.frag");
  shader.use();
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
}

void Ball::setupVAO() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
}

void Ball::render() {
  shader.use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, glm::vec3(SCALE));

  shader.setMat4("model", model);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
}

void Ball::updatePosition(const glm::vec3 newPos) { position = newPos; }

const glm::vec3 &Ball::getPosition() const { return position; }
