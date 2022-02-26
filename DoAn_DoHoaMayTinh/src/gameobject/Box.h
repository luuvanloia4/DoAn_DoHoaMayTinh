﻿#pragma once

#include "../rendering/IDrawable.h"
#include "../rendering/Texture.h"
#include "../resources/RawTexture.h"
#include "../utilities/Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../rendering/ListMaterial.h"
#include "../gameobject/Bullet.h"

//
extern std::vector<glm::vec2> g_KillPosition;

class Box : public IDrawable {
public:
	GLuint m_VBO, m_VAO, m_EBO;
	//Color
	glm::vec3 m_Color;
	bool m_HaveTexture;
	GLuint m_Texture;
	Material m_Material;

	//Position
	glm::vec3 m_Position;
	glm::mat4 m_Model;
	glm::vec3 m_Size;

	//Vat ly:
	bool m_IsGravity;
	float m_Weight;
	bool m_IsRoll;
	float m_RollSpeed;

	//Game
	bool m_IsSpecial;
	int m_Status;
	int m_Type;
	int m_LastBulletTouchIndex;

	static glm::vec3 GenColor(int type) {
		glm::vec3 rs;
		switch (type) {
		case 1:
			//rs = glm::vec3(0.9f, 0.0f, 0.3f);
			rs = glm::vec3(0.9f, 0.9f, 0.9f);
			break;
		case 2:
			rs = glm::vec3(0.0f, 0.9f, 0.5f);
			break;
		case 3:
			rs = glm::vec3(0.0f, 0.8f, 1.0f);
			break;
			//Optional
		case 4:
			rs = glm::vec3(1.0f, 0.5f, 0.2f);
			break;
		case 5:
			rs = glm::vec3(1.0f, 0.2f, 0.5f);
			break;
		case 6:
			rs = glm::vec3(0.5f, 1.0f, 0.2f);
			break;
		case 7:
			rs = glm::vec3(0.2f, 1.0f, 0.5f);
			break;
		case 8:
			rs = glm::vec3(0.5f, 0.2f, 1.0f);
			break;
		case 9:
			rs = glm::vec3(0.2f, 0.5f, 1.0f);
			break;
		default:
			rs = glm::vec3(1.0f);
			break;
		}
		
		return rs;
	}

	//Set default Initialize
	void GenBoxType(int type) {
		m_Status = 0;
		//Color, game attribute
		m_Type = type;
		m_Color = GenColor(m_Type);
	}

	void Initialize() {
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

	Box() {
		//
		m_LastBulletTouchIndex = -1;
		Initialize();
	}

	Box(glm::vec3 position, glm::vec3 color, bool haveTexture, std::string texturePath)
	{
		//Color
		m_Color = color;
		m_HaveTexture = haveTexture;
		if (m_HaveTexture) {
			RawTexture rawTexture(texturePath);
			Texture texture = Texture(rawTexture, Texture::EType::Normals);
			m_Texture = texture.ID();
		}
		//Position
		m_Position = position;
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Size = glm::vec3(1.0f);

		//Vat ly:
		m_IsGravity = false;
		m_Weight = 1.0f;
		m_IsRoll = false;
		m_RollSpeed = 45.0f;

		//Game
		m_IsSpecial = false;
		m_Status = 0;
		m_Type = 1;

		//Khởi tạo hình vuông
		Initialize();
		ListMaterial listMaterial;
		SetMaterial(listMaterial.Silver);
	}

	//For box map
	Box(glm::vec3 position, int type, bool isSpecial) {
		//Default color
		m_HaveTexture = false;
		m_Color = glm::vec3(1.0f);
		m_Status = 0;
		//Game
		m_IsSpecial = isSpecial;
		GenBoxType(type);
		//Position
		m_Position = position;
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Size = glm::vec3(1.0f);

		//Vat ly:
		m_IsGravity = false;
		m_Weight = 1.0f;
		m_IsRoll = false;
		m_RollSpeed = 45.0f;

		//Khởi tạo hình vuông
		Initialize();
		ListMaterial listMaterial;
		if (m_IsSpecial) {
			SetMaterial(listMaterial.Ruby);
		}
		else {
			SetMaterial(listMaterial.Silver);
		}
	}

	~Box() override{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void Draw(const ShaderProgram& shader) override{
		//Check touch
		//std::cout << "Draw box!!!" << std::endl;
		bool isTouch = false;
		for (auto bullet : g_BulletQueue) {
			if (m_LastBulletTouchIndex < bullet->m_Index)
			{
				if (bullet->m_Status != -1 && IsTouch(bullet->m_Position, bullet->m_Size)) {
					m_LastBulletTouchIndex = bullet->m_Index;

					if (m_Type == bullet->m_Type) {
						m_Status = -1;
					}
					else {
						m_Type = bullet->m_Type;
						m_Color = GenColor(m_Type);
					}

					std::cout << "In site" << std::endl;
					bullet->m_Status = -1;
					isTouch = true;
					break;
				}
			}
		}

		if (!isTouch) {
			shader.Uniform("uniColor", m_Color);

			//
			shader.Uniform("uniTexture", 0);
			glActiveTexture(GL_TEXTURE0); // Texture unit 0
			if (m_HaveTexture) {
				glBindTexture(GL_TEXTURE_2D, m_Texture);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			m_Material.Bind(shader);

			glm::mat4 processModel = glm::transpose(glm::inverse(m_Model));
			shader.Uniform("processNormal", processModel);
			shader.Uniform("model", m_Model);

			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	bool IsTouch(glm::vec3 pos, glm::vec3 posSize){
		glm::vec3 d = m_Position - pos;
		if (abs(d.x*2) < m_Size.x + posSize.x && abs(d.y*2) < m_Size.y + posSize.y && abs(d.z*2) < m_Size.z + posSize.z) {
			return true;
		}
		
		return false;
	}

	//Thay đổi model:
	void SetSize(glm::vec3 size) {
		m_Model = glm::scale(m_Model, glm::vec3(1 / m_Size.x, 1 / m_Size.y, 1 / m_Size.z));
		//Thu về kích cỡ 1, 1, 1
		m_Size = size;
		m_Model = glm::scale(m_Model, m_Size);
	}

	void Move(glm::vec3 vec) {
		m_Position += vec;
		MoveTo(m_Position);
	}

	void MoveTo(glm::vec3 pos) {
		m_Position = pos;
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Model = glm::scale(m_Model, m_Size);
	}

	void SetMaterial(Material material) {
		m_Material = material;
	}
};

//Data
extern Box* g_BoxMap[31][20]; //Thừa 1 hàng để preload 
extern int g_MapWidth;
extern int g_MapHeight;
extern int g_MapHeight_Max;
//
extern float g_BoxSize;
extern float g_BoxMargin;



