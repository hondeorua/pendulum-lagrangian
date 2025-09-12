#include "ball.hpp"
#include "shader.hpp" //this header file already has the other #include
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>

constexpr unsigned int resolution = 50;
constexpr unsigned int WINDOW_WIDTH = 1200;
constexpr unsigned int WINDOW_HEIGHT = 800;
constexpr float SMALL_SCALE = 0.005f;
constexpr float LARGE_SCALE = 0.01f;

constexpr float LENGTH_OF_ROD = 0.2f;

constexpr float aspect_h_over_w =
    static_cast<float>(WINDOW_HEIGHT) / WINDOW_WIDTH;

constexpr glm::vec3 gravity = glm::vec3(0, -9.81, 0);

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

struct Rod {
  float vertices[4];
};

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

  unsigned int VAO_pivot, VBO_pivot;
  glGenVertexArrays(1, &VAO_pivot);
  glGenBuffers(1, &VBO_pivot);

  glBindVertexArray(VAO_pivot);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_pivot);
  glBufferData(GL_ARRAY_BUFFER, pivot_point.vertices.size() * sizeof(float),
               pivot_point.vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Ball mass1(resolution, aspect_h_over_w);
  pivot_point.position = glm::vec3(0, 0, 0);

  unsigned int VAO_mass1, VBO_mass1;
  glGenVertexArrays(1, &VAO_mass1);
  glGenBuffers(1, &VBO_mass1);

  glBindVertexArray(VAO_mass1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_mass1);
  glBufferData(GL_ARRAY_BUFFER, mass1.vertices.size() * sizeof(float),
               mass1.vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  pivot_point.position = glm::vec3(0, 0.5, 0);
  pivot_point.aTrans = glm::translate(pivot_point.aTrans, pivot_point.position);
  pivot_point.aTrans =
      glm::scale(pivot_point.aTrans, glm::vec3(1) * SMALL_SCALE);

  mass1.position = glm::vec3(0, 0.0, 0);
  mass1.aTrans = glm::translate(mass1.aTrans, mass1.position);
  mass1.aTrans = glm::scale(mass1.aTrans, glm::vec3(1) * LARGE_SCALE);

  float lines[4] = {
      pivot_point.position.x,
      pivot_point.position.y,
      mass1.position.x,
      mass1.position.y,
  };

  unsigned int VAO_lines, VBO_lines;
  glGenVertexArrays(1, &VAO_lines);
  glGenBuffers(1, &VBO_lines);

  glBindVertexArray(VAO_lines);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  float last_time = glfwGetTime();
  glfwSwapInterval(0);
  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float current_time = glfwGetTime();
    float dt = current_time - last_time;
    last_time = current_time;

    shader.use();
    shader.setFloat3f("aColor", mass1.color);

    glBindVertexArray(VAO_pivot);
    shader.setMatrix4fv("aTrans", pivot_point.aTrans);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);

    glBindVertexArray(VAO_mass1);
    shader.setMatrix4fv("aTrans", mass1.aTrans);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);

    glBindVertexArray(VAO_lines);
    glm::mat4 aTrans = glm::mat4(1.0f);
    shader.setMatrix4fv("aTrans", aTrans);
    glm::vec3 rod_color =  glm::vec3(0.2f, 0.2f, 0.2f);
    shader.setFloat3f("aColor", rod_color);
    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width * 2, height * 2);
}
