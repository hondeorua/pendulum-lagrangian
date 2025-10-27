#version 330 core
layout (location = 0) in vec2 aPos;

// out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
   // gl_Position =  model * vec4(aPos, 0.0, 1.0); 
   // vColor = gl_Position.xyz;
}
