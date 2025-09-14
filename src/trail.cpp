#include "trail.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "shader.hpp"

Trail::Trail(int length_of_trail, int resolution, float aspect_h_over_w,
             float SCALE, Ball& ball_to_be_trailed)
    : length(length_of_trail), ball_of_trail(Ball(resolution, aspect_h_over_w)),
      scale(SCALE), resolution(resolution), ball_to_trail(&ball_to_be_trailed) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, ball_of_trail.vertices.size() * sizeof(float), ball_of_trail.vertices.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  // ball_of_trail.color *= 0.5;
}

void Trail::updateTrail() {
  trail_position.push_back((*ball_to_trail).position);
  if (trail_position.size() > length)
    trail_position.pop_front();
}

void Trail::draw(Shader &shader) {
  for (int i = 0; i < trail_position.size(); i++) {
    glm::mat4 aTrans(1.0f);
    aTrans = glm::translate(aTrans, trail_position[i]);
    aTrans = glm::scale(aTrans, glm::vec3(1.0f) * scale);

    float alpha = static_cast<float>(i+1) / length;
    shader.setMatrix4fv("aTrans", aTrans);
    shader.setFloat4f("color", ball_of_trail.color, alpha);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
  }
}
