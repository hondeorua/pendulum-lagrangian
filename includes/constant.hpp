#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr unsigned int resolution = 20;
constexpr double PI = 3.14159265358979323846;
constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH = 800;

constexpr glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5f));
const glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
