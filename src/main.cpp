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
void updatePositionFromTheta(unsigned int order, Ball *balls, float *theta);
std::vector<float> dydt(std::vector<float> y);
std::vector<float> rk4(std::vector<float> &y, float h);

std::vector<float> multiplyVector(const std::vector<float> &vect, float k);
std::vector<float> addVector(const std::vector<float> &vect1,
                             const std::vector<float> &vect2);

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

  std::vector<float> y = {theta[0], theta[1], 0, 0};

  float prev_t = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    float curr_t = glfwGetTime();
    float dt = curr_t - prev_t;
    prev_t = curr_t;

    y = rk4(y, dt);

    theta[0] = y[0];
    theta[1] = y[1];
    updatePositionFromTheta(1, balls, theta);
    updatePositionFromTheta(2, balls, theta);

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
    balls[1].updatePosition(balls[0].getPosition() +
                            ROD_LENGTH *
                                glm::vec3(sin(theta[0]), -cos(theta[0]), 0));
    break;
  case 2:
    balls[2].updatePosition(balls[1].getPosition() +
                            ROD_LENGTH *
                                glm::vec3(sin(theta[1]), -cos(theta[1]), 0));
    break;
  }
}

std::vector<float> dydt(std::vector<float> y) {
  std::vector<float> ret(4);
  ret[0] = y[2];
  ret[1] = y[3];

  float numerator1 =
      ROD_LENGTH * y[2] * y[2] * sin(y[0] - y[1]) * cos(y[0] - y[1]) -
      GRAVITY * sin(y[1]) * cos(y[0] - y[1]) + 2 * GRAVITY * sin(y[0]) +
      ROD_LENGTH * y[3] * y[3] * sin(y[0] - y[1]);
  float numerator2 =
      2 * GRAVITY * sin(y[0]) * cos(y[0] - y[1]) +
      ROD_LENGTH * y[3] * y[3] * sin(y[0] - y[1]) * cos(y[0] - y[1]) +
      2 * ROD_LENGTH * y[2] * y[2] * sin(y[0] - y[1]) - 2 * GRAVITY * sin(y[1]);
  float denomerator1 = ROD_LENGTH * pow(cos(y[0] - y[1]), 2) - 2 * ROD_LENGTH;
  float denomerator2 = -denomerator1;

  ret[2] = numerator1 / denomerator1;
  ret[3] = numerator2 / denomerator2;

  return ret;
}

std::vector<float> rk4(std::vector<float> &y, float h) {
  std::vector<float> k1, k2, k3, k4;
  k1 = dydt(y);
  k2 = dydt(addVector(y, multiplyVector(k1, h * 0.5)));
  k3 = dydt(addVector(y, multiplyVector(k2, h * 0.5)));
  k4 = dydt(addVector(y, multiplyVector(k3, h)));
  return addVector(
      y, multiplyVector(
             addVector(k1, addVector(multiplyVector(k2, 2),
                                     addVector(multiplyVector(k3, 2), k4))),
             h / 6));
}

std::vector<float> multiplyVector(const std::vector<float> &vect, float k) {
  std::vector<float> ret(4);
  ret[0] = vect[0] * k;
  ret[1] = vect[1] * k;
  ret[2] = vect[2] * k;
  ret[3] = vect[3] * k;
  return ret;
}
std::vector<float> addVector(const std::vector<float> &vect1,
                             const std::vector<float> &vect2) {
  std::vector<float> ret(4);
  ret[0] = vect1[0] + vect2[0];
  ret[1] = vect1[1] + vect2[1];
  ret[2] = vect1[2] + vect2[2];
  ret[3] = vect1[3] + vect2[3];
  return ret;
}
