#ifndef Cuboid_h
#define Cuboid_h

#include "../IDrawable.h"
#include "../Texture.h"
#include "../../resources/RawTexture.h"
#include "../ListMaterial.h"

#include <glm/gtc/matrix_transform.hpp>

class Cuboid : public IDrawable {
public:
    Cuboid(glm::mat4 model, glm::vec3 color);
	Cuboid(glm::mat4 model, glm::vec3 color, bool haveTexture, std::string texturePath, bool isRoll, glm::vec3 rollVec, float rollSpeed);

    Cuboid() = delete;
    Cuboid(const Cuboid&) = delete;
    Cuboid& operator=(const Cuboid&) = delete;
    Cuboid(Cuboid&&) = delete;
    Cuboid& operator=(Cuboid&&) = delete;
    ~Cuboid();

    void Draw(const ShaderProgram& shader) override;
	void SetMaterial(Material material);

private:
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
	bool m_IsRoll;
	glm::vec3 m_RollVec;
	float m_RollSpeed;

	void Initialize();
};

#endif
