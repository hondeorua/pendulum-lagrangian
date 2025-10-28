#include "ball.hpp"
#include "constant.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "rod.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void updatePositionFromTheta(unsigned int order, Ball *balls, float *theta);

int main() {
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "May thang mien tay", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Ball balls[3];
  float theta[2] = {PI / 2, PI};

  balls[0].updatePosition(glm::vec3(0, BALL_0_Y, 0));

  updatePositionFromTheta(1, balls, theta);
  updatePositionFromTheta(2, balls, theta);

  std::vector<Rod> rods;
  rods.emplace_back(balls[0], balls[1]);
  rods.emplace_back(balls[1], balls[2]);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 3; ++i) {
      balls[i].render();
    }

    for (int i = 0; i < 2; ++i) {
      rods[i].render();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void updatePositionFromTheta(unsigned int order, Ball *balls, float *theta) {
  switch (order) {
  case 1:
    balls[1].updatePosition(balls[0].getPosition() + ROD_LENGTH * glm::vec3(sin(theta[0]), cos(theta[0]), 0));
    break;
  case 2:
    balls[2].updatePosition(balls[1].getPosition() + ROD_LENGTH * glm::vec3(sin(theta[1]), -cos(theta[1]), 0));
    break;
  }
}
