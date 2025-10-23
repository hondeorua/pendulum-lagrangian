#pragma once

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class Shader {
private:
  unsigned int shaderID;
  void CreateShaderProgram(unsigned int &vertex, unsigned int &fragment);
  void CreateShader(const char *source, unsigned int &ID,
                    std::string shaderType);
public:
  Shader(const char *vertexPath, const char *fragmentPath);
  void use();
  void setBool(const char *var_name, bool value);
  void setFloat(const char *var_name, float value);
  void setInt(const char *var_name, int value);
};
