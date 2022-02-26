#ifndef Plane_h
#define Plane_h

#include "../IDrawable.h"
#include "../Texture.h"
#include "../../resources/RawTexture.h"

class Plane : public IDrawable {
public:
    void Initialize();
    Plane(glm::mat4 model, glm::vec3 color);
    Plane(glm::mat4 model, glm::vec3 color, bool haveTexture, std::string texturePath);

    Plane() = delete;
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;
    Plane(Plane&&) = delete;
    Plane& operator=(Plane&&) = delete;
    ~Plane();

    void Draw(const ShaderProgram& shader) override;
    void SetMaterial(Material material);

private:
    GLuint m_VBO, m_VAO;

    //Color
    glm::vec3 m_Color;
    Material m_Material;
    bool m_HaveTexture;
    GLuint m_Texture;

    //Position
    glm::mat4 m_Model;
    glm::vec3 m_Position;
};

#endif
