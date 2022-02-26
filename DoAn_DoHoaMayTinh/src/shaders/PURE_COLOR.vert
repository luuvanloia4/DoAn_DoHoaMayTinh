#version 330 core

//input
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

//uniform
uniform mat4 model;
uniform mat4 pv; // projection * view
uniform mat4 processNormal;

//output
out vec3 localNormal;
out vec2 localTexture;
out vec3 fragPos;

void main() {
    gl_Position = pv * model * vec4(inPos, 1.0f);
    //
    localNormal = vec3(processNormal * vec4(inNormal, 1.0f));
    localTexture = inTexture;
    fragPos = vec3(model * vec4(inPos, 1.0f));
}
