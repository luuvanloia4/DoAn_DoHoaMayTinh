#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include "ShaderProgram.h"

class Material
{
public:
	vec3 i_Ambient;
	vec3 i_Diffuse;
	vec3 i_Specular;
	float i_Shininess;

	Material() {
		//
	}

	Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, float _shininess) {
		this->i_Ambient = _ambient;
		this->i_Diffuse = _diffuse;
		this->i_Specular = _specular;
		this->i_Shininess = _shininess;
	}
	
	void Bind(const ShaderProgram &shader) {
		shader.Uniform("material.ambient", i_Ambient);
		shader.Uniform("material.diffuse", i_Diffuse);
		shader.Uniform("material.specular", i_Specular);
		shader.Uniform("material.shininess", i_Shininess);
	}
};

