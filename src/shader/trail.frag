#version 330 core
out vec4 FragColor;

in vec3 vColor;

void main(){
   FragColor = vec4(0.5, 0.5, 0.5, 1.0);
   // FragColor = vec4(abs(vColor), 1.0);
}
