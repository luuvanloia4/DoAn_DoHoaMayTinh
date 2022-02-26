#include "Sphere.h"
#include "SphereData.h"

Sphere::Sphere(glm::mat4 model, glm::vec3 color)
    : m_Model(model) {
    

}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Sphere::Draw(const ShaderProgram& shader) {

}
