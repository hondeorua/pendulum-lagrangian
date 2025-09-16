#version 330 core
out vec4 FragColor;

in float alpha;

void main(){
   FragColor = vec4(1.0, 0.235f, 0.48, alpha);
}
