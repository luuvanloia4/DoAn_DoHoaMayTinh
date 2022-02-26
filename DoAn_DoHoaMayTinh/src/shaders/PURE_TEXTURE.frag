#version 330 core

//Input
in vec3 localColor;
in vec2 localTexture;

//Uniform
uniform sampler2D fileTexture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

//Output
out vec4 FragColor;

void main() {
    vec4 resultTexture = texture(fileTexture, localTexture);
    if(resultTexture.x < 0.1f && resultTexture.y < 0.1f && resultTexture.z < 0.1f){
        FragColor = vec4(localColor, 1.0f);
    }
    else{
        FragColor = resultTexture;
    }
}
