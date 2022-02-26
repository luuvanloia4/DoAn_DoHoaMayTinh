#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : m_Direction(direction)
    , m_Ambient(ambient)
    , m_Diffuse(diffuse)
    , m_Specular(specular)
    , m_Color(glm::vec3(1.0f))
{
}

void DirectionalLight::Initialize() {
    Object().Scene().RegisterShaderProperty(this, EShaderType::Phong);
    Object().Scene().RegisterShaderProperty(this, EShaderType::PureColor);
}

void DirectionalLight::Destroy() {
    Object().Scene().UnregisterShaderProperty(this, EShaderType::Phong);
}

void DirectionalLight::SetProperty(const ShaderProgram& shader) const {
    shader.Uniform("dirLight.direction", m_Direction);
    shader.Uniform("dirLight.color", m_Color);
    shader.Uniform("dirLight.ambient", m_Ambient);
    shader.Uniform("dirLight.diffuse", m_Diffuse);
    shader.Uniform("dirLight.specular", m_Specular);
}
