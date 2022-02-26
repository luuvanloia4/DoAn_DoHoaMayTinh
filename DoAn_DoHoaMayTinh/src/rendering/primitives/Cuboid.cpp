#include "Cuboid.h"

//A -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
//B -0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
//C -0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
//D -0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
//E  0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
//F  0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
//G  0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
//H  0.5f,  0.5f,  0.5f, color.x, color.y, color.z,

//Set default Initialize
void Cuboid::Initialize() {
	GLfloat vertices[] = {
		//Toa do dinh			//Texture			//Vector phap tuyen
		//Bottom
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		//Top
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//Left
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		//Right
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		//Back
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		//Front
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,			0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f
	};

	/*GLint drawIndex[] = {
		0, 1, 2, 0, 3, 2,
		4, 5, 6, 4, 7, 6,
		8, 9, 10, 8, 11, 10,
		12, 13, 14, 12, 15, 14,
		16, 17, 18, 16, 19, 18,
		20, 21, 22, 20, 23, 22
	};*/

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//	Vector phap tuyen
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);*/
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawIndex), drawIndex, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Cuboid::Cuboid(glm::mat4 model, glm::vec3 color)
    : m_Model(model), m_Color(color)
{
	Initialize();
}

Cuboid::Cuboid(glm::mat4 model, glm::vec3 color, bool haveTexture, std::string texturePath, bool isRoll, glm::vec3 rollVec, float rollSpeed) {
	//Color;
	m_HaveTexture = haveTexture;
	if (m_HaveTexture) {
		RawTexture localRawTexture(texturePath);
		Texture localTexture(localRawTexture, (Texture::EType)5);
		m_Texture = localTexture.ID();
	}
	m_Color = color;

	//Position
	m_Model = model;
	m_Size = glm::vec3(m_Model[0][0], m_Model[1][1], m_Model[2][2]);
	m_Position = glm::vec3(m_Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//Vat ly
	m_IsRoll = isRoll;
	m_RollVec = rollVec;
	m_RollSpeed = rollSpeed;

	Initialize();
}

Cuboid::~Cuboid() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Cuboid::Draw(const ShaderProgram& shader) {
	//Color
	if (m_HaveTexture) {
		shader.Uniform("uniTexture", 0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	shader.Uniform("uniColor", m_Color);
	
	//Positiion
	if (m_IsRoll) {
		this->m_Model = glm::rotate(m_Model, glm::radians(m_RollSpeed), m_RollVec);
	}
	glm::mat4 processModel = glm::transpose(glm::inverse(m_Model));
	shader.Uniform("processNormal", processModel);
    shader.Uniform("model", m_Model);

	m_Material.Bind(shader);

	//Draw
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cuboid::SetMaterial(Material material) {
	m_Material = material;
}