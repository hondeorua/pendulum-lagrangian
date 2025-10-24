#include "ball.hpp"
#include "constant.hpp"

Ball::Ball() {
  vertices[0] = 0.0;
  vertices[1] = 0.0;
  for (unsigned int i = 1; i <= resolution; ++i) {
    double theta = 2 * PI * i / resolution;
    vertices[2 * i] = std::cos(theta);
    vertices[2 * i + 1] = std::sin(theta);
  }
  vertices[2 * resolution + 2] = vertices[2];
  vertices[2 * resolution + 3] = vertices[3];
  setupVAO();
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

void Ball::render(Shader &shader) {
  shader.use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
}
