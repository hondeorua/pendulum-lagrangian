#version 330 core
out vec4 FragColor;

uniform vec3 aColor;

void main(){
   FragColor = vec4(aColor, 1.0);
   // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
