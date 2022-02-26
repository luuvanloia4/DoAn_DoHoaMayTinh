#ifndef DirectionalLight_h
#define DirectionalLight_h

#include "Component.h"
#include "../../scenes/Scene.h"
#include "../../rendering/IShaderProperty.h"

class DirectionalLight : public Component, public IShaderProperty {
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void Initialize() override;
    void Destroy() override;

    void SetProperty(const ShaderProgram& shader) const override;

private:
    glm::vec3 m_Direction;
    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    glm::vec3 m_Color;
};

#endif
