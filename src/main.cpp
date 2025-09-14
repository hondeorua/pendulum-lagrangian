#include "ball.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "rod.hpp"
#include "shader.hpp" //this header file already has the other #include
#include "trail.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <ostream>

constexpr unsigned int resolution = 30;
constexpr unsigned int WINDOW_WIDTH = 1000;
constexpr unsigned int WINDOW_HEIGHT = 1000;
constexpr float SMALL_SCALE = 0.005f;
constexpr float LARGE_SCALE = 0.015f;
constexpr unsigned int NUM_BALL = 5;
constexpr float mass[] = {10000, 25.0, 3.5, 1.3, 0.5};

// width of rod set to half a ball's diameter
constexpr float WIDTH_OF_ROD = SMALL_SCALE;
constexpr float LENGTH_OF_ROD = 0.2f;

constexpr unsigned int LENGTH_OF_TRAIL = 1000;

constexpr float aspect_h_over_w =
    static_cast<float>(WINDOW_HEIGHT) / WINDOW_WIDTH;

constexpr glm::vec3 gravity = glm::vec3(0, -9.81, 0);

GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void enforce_constraints(Ball &ball1, Ball &ball2);

int main() {

  std::cout << "program's running ech ech..." << '\n';

  if (!glfwInit()) {
    std::cout << "no gud glfw not inited gudly" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "pendulum", NULL, NULL);
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

  Shader shader_for_particle("../src/shaders/shader-for-particle.vert",
                             "../src/shaders/shader-for-particle.frag");
  Shader shader_for_rod("../src/shaders/shader-for-rod.vert",
                        "../src/shaders/shader-for-rod.frag");
  Shader shader_for_trail("../src/shaders/shader-for-trail.vert",
                          "../src/shaders/shader-for-trail.frag");

  std::vector<Ball> balls;
  balls.reserve(NUM_BALL);
  std::vector<unsigned int> VAOs(NUM_BALL);
  std::vector<unsigned int> VBOs(NUM_BALL);

  for (int i = 0; i < NUM_BALL; ++i) {
    balls.emplace_back(resolution, aspect_h_over_w, mass[i]);

    glGenVertexArrays(1, &VAOs[i]);
    glGenBuffers(1, &VBOs[i]);

    glBindVertexArray(VAOs[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
    glBufferData(GL_ARRAY_BUFFER, balls[i].vertices.size() * sizeof(float),
                 balls[i].vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  balls[0].isPivot = true;

  balls[0].position = glm::vec3(0, 0.5, 0);
  balls[0].aTrans = glm::translate(balls[0].aTrans, balls[0].position);
  balls[0].aTrans = glm::scale(balls[0].aTrans, glm::vec3(1) * SMALL_SCALE);

  balls[NUM_BALL - 1].color = glm::vec3(1.0, 0.235f, 0.48);

  for (int i = 1; i < NUM_BALL; ++i) {
    balls[i].position = balls[i - 1].position + glm::vec3(-LENGTH_OF_ROD, 0, 0);
  }

  std::vector<Rod> rods;
  rods.reserve(NUM_BALL - 1);
  for (int i = 0; i < NUM_BALL - 1; i++) {
    rods.emplace_back(balls[i], balls[i + 1], LENGTH_OF_ROD, WIDTH_OF_ROD);
  }

  rods[0].length *= 1.5;

  Trail trail(LENGTH_OF_TRAIL, resolution, aspect_h_over_w, SMALL_SCALE / 10,
              balls[balls.size() - 1]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float last_time = glfwGetTime();
  glfwSwapInterval(0);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float current_time = glfwGetTime();
    float dt = current_time - last_time;
    last_time = current_time;

    for (Rod &rod : rods) {
      rod.updatePosition();
      rod.draw(shader_for_rod);
    }

    shader_for_particle.use();
    shader_for_particle.setMatrix4fv("aTrans", balls[0].aTrans);
    shader_for_particle.setFloat3f("color", balls[0].color);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);

    std::vector<glm::vec3> temp(NUM_BALL - 1);

    for (int i = 1; i < NUM_BALL; ++i) {
      balls[i].velocity += gravity * dt;

      temp[i - 1] = balls[i].position;
      balls[i].position += balls[i].velocity * dt;
    }

    for (int j = 0; j < 5; j++) {
      for (Rod &rod : rods) {
        rod.enforce_length();
      }
    }

    for (int i = 1; i < NUM_BALL; i++) {
      balls[i].velocity = (balls[i].position - temp[i - 1]) / dt;

      balls[i].aTrans = glm::mat4(1.0f);
      balls[i].aTrans = glm::translate(balls[i].aTrans, balls[i].position);
      if (i < NUM_BALL - 1)
        balls[i].aTrans =
            glm::scale(balls[i].aTrans, glm::vec3(1) * SMALL_SCALE);
      else
        balls[i].aTrans =
            glm::scale(balls[i].aTrans, glm::vec3(1) * LARGE_SCALE);

      glBindVertexArray(VAOs[i]);
      shader_for_particle.setMatrix4fv("aTrans", balls[i].aTrans);
      shader_for_particle.setFloat3f("color", balls[i].color);
      glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
    }

    trail.updateTrail();
    trail.draw(shader_for_trail);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
