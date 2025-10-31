#include "trail.hpp"
#include "ball.hpp"
#include "constant.hpp"
#include <iterator>

Shader Trail::shader;

Trail::Trail(Ball &toTrack) : tracked(toTrack) {
  updateTrail();

  shader.makeShader("../src/shader/trail.vert", "../src/shader/trail.frag");
  shader.use();
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
}

void Trail::render() {
  shader.use();

  glBindVertexArray(VAO);
  glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)trail.size());
}

void Trail::updateTrail() {
  trail.push_back(tracked.getPosition());
  if (trail.size() > MAX_LENGTH_TRAIL)
    trail.pop_front();
  setupVAO();
}

void Trail::setupVAO() {
  std::vector<float> vertices;
  vertices.reserve(trail.size() * 2);
  for (const auto &p : trail) {
    vertices.push_back(p.x);
    vertices.push_back(p.y);
  }

  if (VAO == 0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
  } else {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);
  }
}
