#pragma once

#include <GLFW/glfw3.h>

class Shader{
private:
   unsigned int shaderID;
   unsigned int InitShader();
public:
   Shader(const char* vertexPath, const char* FragmentPath);
   void use();
};
