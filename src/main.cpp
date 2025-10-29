#include "ball.hpp"
#include "constant.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float4.hpp"
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
void updatePosition(Ball *balls, float *theta);
glm::vec4 dydt(glm::vec4 &y);
glm::vec4 rk4(glm::vec4 &y, float h);

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

  updatePosition(balls, theta);

  std::vector<Rod> rods;
  rods.emplace_back(balls[0], balls[1]);
  rods.emplace_back(balls[1], balls[2]);

  glm::vec4 y = {theta[0], theta[1], 0, 0};

  float prev_t = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    float curr_t = glfwGetTime();
    float dt = curr_t - prev_t;
    prev_t = curr_t;

    glClear(GL_COLOR_BUFFER_BIT);

    y = rk4(y, dt);

    theta[0] = y[0];
    theta[1] = y[1];
    updatePosition(balls, theta);

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

void updatePosition(Ball *balls, float *theta) {
  balls[1].updatePosition(balls[0].getPosition() +
                          ROD_LENGTH *
                              glm::vec3(sin(theta[0]), -cos(theta[0]), 0));
  balls[2].updatePosition(balls[1].getPosition() +
                          ROD_LENGTH *
                              glm::vec3(sin(theta[1]), -cos(theta[1]), 0));
}

glm::vec4 dydt(glm::vec4 f) {
  float numerator1 =
      ROD_LENGTH * f[2] * f[2] * sin(f[0] - f[1]) * cos(f[0] - f[1]) -
      GRAVITY * sin(f[1]) * cos(f[0] - f[1]) + 2 * GRAVITY * sin(f[0]) +
      ROD_LENGTH * f[3] * f[3] * sin(f[0] - f[1]);
  float numerator2 =
      2 * GRAVITY * sin(f[0]) * cos(f[0] - f[1]) +
      ROD_LENGTH * f[3] * f[3] * sin(f[0] - f[1]) * cos(f[0] - f[1]) +
      2 * ROD_LENGTH * f[2] * f[2] * sin(f[0] - f[1]) - 2 * GRAVITY * sin(f[1]);
  float denomerator1 = ROD_LENGTH * pow(cos(f[0] - f[1]), 2) - 2 * ROD_LENGTH;
  float denomerator2 = -denomerator1;

  return glm::vec4{f[2], f[3], numerator1 / denomerator1,
                   numerator2 / denomerator2};
}

glm::vec4 rk4(glm::vec4 &y, float h) {
  glm::vec4 k1, k2, k3, k4;
  k1 = dydt(y * 1.0f);
  k2 = dydt(y + k1 * h * 0.5f);
  k3 = dydt(y + k2 * h * 0.5f);
  k4 = dydt(y + k3 * h);
  return y + (k1 + 2.0f * k2 + 2.0f * k3 + k4) * (h / 6);
}
