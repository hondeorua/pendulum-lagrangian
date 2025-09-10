#include "glm/geometric.hpp"
#include "shader.hpp" //this header file already has the other #include
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

constexpr glm::vec3 SCALE = glm::vec3(1.0f, 1.0f, 1.0f) * 0.05f;

constexpr glm::vec3 gravity = glm::vec3(0, -9.81, 0);

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
glm::vec3 normal_vect_wall(glm::vec3 position);
bool hit_wall(glm::vec3 position);

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

  unsigned int resolution = 50;

  float vertices[resolution * 2 + 4];
  vertices[0] = 0.0;
  vertices[1] = 0.0;
  for (int i = 1; i < resolution + 1; i++) {
    float theta = 2 * M_PI * (i - 1) / resolution;
    vertices[i * 2] = std::cos(theta) * WINDOW_HEIGHT / WINDOW_WIDTH;
    vertices[i * 2 + 1] = std::sin(theta);
  }
  vertices[resolution * 2 + 2] =
      static_cast<float>(WINDOW_HEIGHT) / WINDOW_WIDTH;
  vertices[resolution * 2 + 3] = 0.0;

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::vec3 position = glm::vec3(0, 0, 0);
  glm::vec3 velocity = glm::vec3(0.5, 0.7, 0);
  unsigned int transLoc = glGetUniformLocation(shader.shaderID, "aTrans");

  float last_time = glfwGetTime();

  glfwSwapInterval(0);
  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float current_time = glfwGetTime();
    float delta_t = current_time - last_time;
    last_time = current_time;

    if (hit_wall(position)) {
      velocity = glm::reflect(velocity, normal_vect_wall(position));
    }

    velocity += gravity * delta_t;

    position +=
        velocity * delta_t +
        0.5f * gravity * (current_time * current_time - last_time * last_time);
    glm::mat4 transformation_mat = glm::mat4(1.0);
    transformation_mat = glm::translate(transformation_mat, position);
    transformation_mat = glm::scale(transformation_mat, SCALE);

    shader.use();
    glUniformMatrix4fv(transLoc, 1, GL_FALSE,
                       glm::value_ptr(transformation_mat));

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

glm::vec3 normal_vect_wall(glm::vec3 position) {
  float x = position.x;
  float y = position.y;
  if (std::abs(x) > std::abs(y)) {
    if (x > 0)
      return glm::vec3(-1, 0, 0);
    if (x < 0)
      return glm::vec3(1, 0, 0);
  }
  if (y > 0)
    return glm::vec3(0, -1, 0);
  if (y < 0)
    return glm::vec3(0, 1, 0);
  return glm::normalize(glm::vec3(-x / std::abs(x), -y / std::abs(y), 0));
}

bool hit_wall(glm::vec3 position) {
  float x = position.x;
  float y = position.y;
  if (std::abs(x) >= 0.95 || std::abs(y) >= 0.95)
    return true;
  return false;
}
