#ifndef SHADER_HPP
#define SHADER_HPP

#include <cstdlib>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

class Shader {
public:
  unsigned int shaderID;

  Shader(const char *vertexPath, const char *fragmentPath);
  void use();

  // utilities
  void setBool(const char *name, bool value) const;
  void setInt(const char *name, int value) const;
  void setFloat(const char *name, float value) const;
  void setFloat3f(const char *name, glm::vec3 &value) const;
  void setMatrix4fv(const char *name, glm::mat4 &value) const;
};

#endif
