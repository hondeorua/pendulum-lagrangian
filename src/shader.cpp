#include <glad/glad.h>
#include "shader.hpp"

void Shader::CreateShaderProgram(unsigned int &vertex, unsigned int &fragment) {
  int success;
  char infoLog[512];

  shaderID = glCreateProgram();
  glAttachShader(shaderID, vertex);
  glAttachShader(shaderID, fragment);
  glLinkProgram(shaderID);

  glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::SHADER::LINKING::FAILED\n"
              << infoLog << std::endl;
  }
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  unsigned int vertex, fragment;
  std::string line;

  // Read vertex source
  std::ifstream vertexIfs{vertexPath};
  if (!vertexIfs.is_open())
    throw std::runtime_error("Cannot open vertex source file");
  std::string vertexCode = "";
  while (std::getline(vertexIfs, line)) {
    vertexCode += '\n' + line;
  }
  const char *vertexCodeCStyle = vertexCode.c_str();

  // Read fragment source
  std::ifstream fragmentIfs{fragmentPath};
  if (!fragmentIfs.is_open())
    throw std::runtime_error("Cannot open fragment source file");
  std::string fragmentCode = "";
  while (std::getline(fragmentIfs, line)) {
    fragmentCode += '\n' + line;
  }
  const char *fragmentCodeCStyle = fragmentCode.c_str();

  CreateShader(vertexCodeCStyle, vertex, "VERTEX");
  CreateShader(fragmentCodeCStyle, fragment, "FRAGMENT");

  CreateShaderProgram(vertex, fragment);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::CreateShader(const char *source, unsigned int &ID,
                          std::string shaderType) {
  int success;
  char infoLog[512];
  ID = glCreateShader((shaderType == "VERTEX") ? GL_VERTEX_SHADER
                                               : GL_FRAGMENT_SHADER);
  glShaderSource(ID, 1, &source, nullptr);
  glCompileShader(ID);
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" + shaderType + "::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
}

void Shader::use() { glUseProgram(shaderID); }
void Shader::setBool(const char *var_name, bool value) {
  glUniform1i(glGetUniformLocation(shaderID, var_name), value);
}
void Shader::setFloat(const char *var_name, float value) {
  glUniform1f(glGetUniformLocation(shaderID, var_name), value);
}
void Shader::setInt(const char *var_name, int value) {
  glUniform1i(glGetUniformLocation(shaderID, var_name), value);
}
