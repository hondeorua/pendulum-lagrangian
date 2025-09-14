#version 330 core
out vec4 FragColor;

in float alpha;
// uniform vec3 color;
// uniform sampler1D fadeTex;

void main(){
   FragColor = vec4(1.0, 0.235f, 0.48, alpha);
}
