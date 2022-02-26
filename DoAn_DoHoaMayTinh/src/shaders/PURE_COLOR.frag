#version 330 core

//input
in vec3 localNormal;
in vec2 localTexture;
in vec3 fragPos;

//uniform
uniform vec3 uniColor;
uniform sampler2D uniTexture;
uniform vec3 viewPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct DirectionLight{
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight dirLight;


//out put
out vec4 outColor;

void main() {
    //Ambient
    vec3 ambient = dirLight.ambient;
    
    //Diffuse
	vec3 norm = normalize(localNormal);
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = dirLight.diffuse * (diff * material.diffuse);
	
	// - Specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = dirLight.specular * (spec * material.specular);

	//=> Light color
	vec3 trueLight = (ambient + diffuse + specular);
	//Load texture
	vec4 textureValue = texture(uniTexture, localTexture);

	if(textureValue.x < 0.01f && textureValue.y < 0.01f && textureValue.z < 0.01f){
		outColor = vec4(uniColor * trueLight, 1.0f);
	}
	else{
		outColor = textureValue * vec4(trueLight, 1.0f);
	}
}
