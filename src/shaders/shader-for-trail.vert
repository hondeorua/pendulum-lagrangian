#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 projection;
uniform int currtrailLength;
out float alpha;

void main() {
   alpha = float(gl_VertexID)/float(currtrailLength);
   gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
