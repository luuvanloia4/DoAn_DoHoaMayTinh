#pragma once
#define _USE_MATH_DEFINES
#include <deque>
#include <math.h>

#include "../rendering/IDrawable.h"
#include "../rendering/Texture.h"
#include "../resources/RawTexture.h"
#include "../utilities/Time.h"
#include "../cbs/components/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../rendering/ListMaterial.h"

extern float g_RedDotSize;
extern int g_BulletCount;

class Bullet : public IDrawable {
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
	glm::vec3 m_Angle;

	//Vat ly:
	bool m_IsGravity;
	float m_Weight;
	bool m_IsRoll;
	float m_RollSpeed;
	glm::vec3 m_Direction;
	bool m_IsFly;
	float m_FlySpeed;

	//Game
	bool m_IsSpecial;
	int m_Status;
	int m_Type;
	int m_Index;
	bool m_IsRedDot = false;

	//Set default Initialize
	void Initialize() {
		m_Index = g_BulletCount++;

		GLfloat vertices[] = {
			//Toa do dinh			//Texture			//Vector phap tuyen
			//Bottom
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			0.0f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
			0.0f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			//Top
			0.0f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			//Left
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,		1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.5f,		0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
			//Right
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
			0.5f, 0.0f, -0.5f,		1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.5f,		0.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
			//Back
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			0.5f, 0.0f, -0.5f,		1.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			0.0f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			0.0f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			//Front
			0.0f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			0.5f, 0.0f, 0.5f,		1.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			-0.5f, 0.0f, 0.5f,		0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			0.0f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			//Triangle bottom
			//Goc phan tu thu 1
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			1.0f, -1.0f, -1.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,			1.0f, -1.0f, -1.0f,
			0.5f, 0.0f, -0.5f,		0.5f, 1.0f,			1.0f, -1.0f, -1.0f,
			//Goc phan tu thu 2
			0.0f, -0.5f, -0.5f,		0.0f, 0.0f,			-1.0f, -1.0f, -1.0f,
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f,			-1.0f, -1.0f, -1.0f,
			-0.5f, 0.0f, -0.5f,		0.5f, 1.0f,			-1.0f, -1.0f, -1.0f,
			//Goc phan tu thu 3
			0.0f, -0.5f, 0.5f,		0.0f, 0.0f,			-1.0f, -1.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f,			-1.0f, -1.0f, 1.0f,
			-0.5f, 0.0f, 0.5f,		0.5f, 1.0f,			-1.0f, -1.0f, 1.0f,
			//Goc phan tu thu 4
			0.0f, -0.5f, 0.5f,		0.0f, 0.0f,			1.0f, -1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,			1.0f, -1.0f, 1.0f,
			0.5f, 0.0f, 0.5f,		0.5f, 1.0f,			1.0f, -1.0f, 1.0f,
			//Triangle top
			//Goc phan tu thu 1
			0.0f, 0.5f, -0.5f,		0.0f, 0.0f,			1.0f, 1.0f, -1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 0.0f,			1.0f, 1.0f, -1.0f,
			0.5f, 0.0f, -0.5f,		0.5f, 1.0f,			1.0f, 1.0f, -1.0f,
			//Goc phan tu thu 2
			0.0f, 0.5f, -0.5f,		0.0f, 0.0f,			-1.0f, 1.0f, -1.0f,
			-0.5f, 0.5f, 0.0f,		1.0f, 0.0f,			-1.0f, 1.0f, -1.0f,
			-0.5f, 0.0f, -0.5f,		0.5f, 1.0f,			-1.0f, 1.0f, -1.0f,
			//Goc phan tu thu 3
			0.0f, 0.5f, 0.5f,		0.0f, 0.0f,			-1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		1.0f, 0.0f,			-1.0f, 1.0f, 1.0f,
			-0.5f, 0.0f, 0.5f,		0.5f, 1.0f,			-1.0f, 1.0f, 1.0f,
			//Goc phan tu thu 4
			0.0f, 0.5f, 0.5f,		0.0f, 0.0f,			1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 0.0f,			1.0f, 1.0f, 1.0f,
			0.5f, 0.0f, 0.5f,		0.5f, 1.0f,			1.0f, 1.0f, 1.0f
		};

		//GLfloat vertices[] = {
		//	//Toa do dinh			//Texture			//Vector phap tuyen
		//	//Bottom
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		//	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		//	0.5f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		//	0.5f, -0.5f, 0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		//	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		//	//Top
		//	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		//	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,			0.0f, 1.0f, 0.0f,
		//	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//	//Left
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		//	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		//	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		//	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		//	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
		//	//Right
		//	0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		//	0.5f, 0.5f, -0.5f,		1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		//	0.5f, -0.5f, 0.5f,		0.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		//	0.5f, -0.5f, -0.5f,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		//	//Back
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		//	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		//	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		//	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		//	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,			0.0f, 0.0f, -1.0f,
		//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
		//	//Front
		//	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
		//	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,			0.0f, 0.0f, 1.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		//	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		//	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f
		//};

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

	Bullet() {
		Initialize();
	}

	Bullet(glm::vec3 color, bool haveTexture, std::string texturePath)
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
		m_Position = glm::vec3(0.0f);
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Size = glm::vec3(0.1f);
		m_Angle = glm::vec3(0.0f, 0.0f, 0.0f);

		//Vat ly:
		m_IsGravity = false;
		m_Weight = 1.0f;
		m_IsRoll = false;
		m_RollSpeed = 360.0f;
		m_Direction = glm::vec3(0.0f);
		m_IsFly = false;
		m_FlySpeed = 5.0f;

		//Game
		m_IsSpecial = false;
		m_Status = 1;
		m_Type = 1;

		//Khởi tạo hình vuông
		Initialize();
		ListMaterial listMaterial;
		SetMaterial(listMaterial.Gold);
	}

	Bullet(Bullet* oldBullet) {
		//Color;
		m_Color = oldBullet->m_Color;
		m_HaveTexture = oldBullet->m_HaveTexture;
		if (m_HaveTexture) {
			m_Texture = oldBullet->m_Texture;
		}
		//Position
		m_Position = oldBullet->m_Position;
		m_Size = glm::vec3(4.0f);
		m_Angle = glm::vec3(0.0f);
		m_Model = glm::translate(glm::mat4(1.0f), m_Position);
		m_Model = glm::scale(m_Model, m_Size);
		//Vat ly
		m_IsGravity = false;
		m_Weight = 1.0f;
		m_IsRoll = false;
		m_RollSpeed = 15.0f;
		m_Direction = oldBullet->m_Direction;
		m_IsFly = true;
		m_FlySpeed = 2.0f;
		//Game
		m_IsSpecial = oldBullet->m_IsSpecial;
		m_Status = 1;
		m_Type = oldBullet->m_Type;
		//Khoi tao
		Initialize();
		ListMaterial listMaterial;
		SetMaterial(listMaterial.Ruby);
	}

	~Bullet() override {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void Draw(const ShaderProgram& shader) override {
		//std::cout << "Draw box!!!" << std::endl;
		shader.Uniform("uniColor", m_Color);
		shader.Uniform("uniTexture", 0);
		glActiveTexture(GL_TEXTURE0); // Texture unit 0
		if (m_HaveTexture) {
			glBindTexture(GL_TEXTURE_2D, m_Texture);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		m_Material.Bind(shader);

		if (m_IsFly) {
			m_Model = glm::translate(m_Model, m_FlySpeed * m_Direction);
			m_Position = glm::vec3(m_Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else {
			if (m_IsRedDot) {
				//Do reddot update position và dir liên tục => model tính lại liên tục nên muốn quay thì phải lưu lại góc quay trước đó
				//Chưa tối ưu lắm
				m_Angle.x += (360.0f * g_Time.DeltaTime() * 0.05f / g_RedDotSize);
				m_Angle.z += (350.0f * g_Time.DeltaTime() * 0.05f / g_RedDotSize);
				m_Model = glm::rotate(m_Model, glm::radians(m_Angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
				//m_Model = glm::rotate(m_Model, glm::radians(m_Angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
				m_Model = glm::rotate(m_Model, glm::radians(m_Angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else {
				float l_Rollspeed = m_RollSpeed * g_Time.DeltaTime();
				m_Model = glm::rotate(m_Model, glm::radians(l_Rollspeed), glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}

		glm::mat4 processModel = glm::transpose(glm::inverse(m_Model));
		shader.Uniform("processNormal", processModel);
		shader.Uniform("model", m_Model);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 60);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Thay đổi model:
	void SetSize(glm::vec3 size) {
		m_Model = glm::scale(m_Model, glm::vec3(1 / m_Size.x, 1 / m_Size.y, 1 / m_Size.z));
		//Thu về kích cỡ 1, 1, 1
		m_Size = size;
		m_Model = glm::scale(m_Model, m_Size);
	}

	void UpdateAngle(float angleX, float angleY, float angleZ) {
		if (!m_IsFly) {
			m_Angle.x += angleX;
			m_Angle.y += angleY;
			m_Angle.z += angleZ;
		}
	}

	void UpdatePosition(glm::vec3 pos, glm::vec3 dir) {
		if(m_IsFly){
			//
		}
		else{
			glm::vec3 direction = glm::normalize(dir);
			m_Position = pos + 3.0f * direction;
			m_Direction = direction;
			m_Model = glm::mat4(1.0f);
			m_Model = glm::translate(m_Model, m_Position);
			m_Size = glm::vec3(g_RedDotSize);
			m_Model = glm::scale(m_Model, m_Size);
		}
		m_Position = glm::vec3(m_Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void SetMaterial(Material material) {
		m_Material = material;
	}

	void MoveTo(glm::vec3 pos) {
		m_Position = pos;
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Model = glm::scale(m_Model, m_Size);
	}
	
	void Move(glm::vec3 vec) {
		m_Model = glm::translate(m_Model, vec);
		m_Position = glm::vec3(m_Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	//Events
	//--Fire
	void Fire() {
		m_IsFly = true;
	}
};

//Global member
extern std::deque<Bullet*> g_BulletQueue;
extern Bullet* g_RedDot;
