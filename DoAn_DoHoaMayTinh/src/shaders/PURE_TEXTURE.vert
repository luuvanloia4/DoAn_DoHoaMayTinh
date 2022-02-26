#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexture;

uniform mat4 model;
uniform mat4 pv; // projection * view

out vec3 localColor;
out vec2 localTexture;

void main() {
    gl_Position = pv * model * vec4(inPos, 1.0f);
    
    //Pass to fragment
    localColor = inColor;
    localTexture = inTexture;
}
