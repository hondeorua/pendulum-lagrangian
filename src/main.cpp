#include "ball.hpp"
#include "shader.hpp" //this header file already has the other #include
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>

constexpr unsigned int resolution = 50;
constexpr unsigned int WINDOW_WIDTH = 1200;
constexpr unsigned int WINDOW_HEIGHT = 800;
constexpr float SCALE = 0.05f;

constexpr float aspect_h_over_w =
    static_cast<float>(WINDOW_HEIGHT) / WINDOW_WIDTH;

constexpr glm::vec3 gravity = glm::vec3(0, -9.81, 0);

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {

  std::cout << "program's running ech ech..." << std::endl;

  if (!glfwInit()) {
    std::cout << "no gud glfw not inited gudly" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mommiii", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cout << "no gud window not created" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");

  Ball pivot_point(resolution, aspect_h_over_w);

  pivot_point.position = glm::vec3(0, 0, 0);

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, pivot_point.vertices.size() * sizeof(float),
               pivot_point.vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::mat4 aTrans = glm::mat4(1.0f);
  aTrans = glm::scale(aTrans, glm::vec3(1, 1, 1) * SCALE);

  float last_time = glfwGetTime();

  glfwSwapInterval(0);
  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float current_time = glfwGetTime();
    float dt = current_time - last_time;
    last_time = current_time;

    shader.use();
    shader.setMatrix4fv("aTrans", aTrans);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width * 2, height * 2);
}
