#include "Cube.h"

Cube::Cube(const glm::vec3& color) {
    float vertices[] = {
     -0.5f, -0.5f, -0.5f,color.x, color.y, color.z,
     0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
     0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
     0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,

    -0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
     0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f,  0.5f, color.x, color.y, color.z,

    -0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f,  0.5f, color.x, color.y, color.z,

     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
     0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
     0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
     0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
     0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,

    -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
     0.5f, -0.5f, -0.5f, color.x, color.y, color.z,
     0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
     0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f, -0.5f, -0.5f, color.x, color.y, color.z,

    -0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
     0.5f,  0.5f, -0.5f, color.x, color.y, color.z,
     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
     0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f,  0.5f, color.x, color.y, color.z,
    -0.5f,  0.5f, -0.5f, color.x, color.y, color.z
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::Draw(const ShaderProgram& shader) {
    shader.Uniform("model", TransformIn.Value()->Model());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Initialize() {
    Object().Scene().RegisterDrawCall(this, EShaderType::PureColor);
}

void Cube::Destroy() {
    Object().Scene().UnregisterDrawCall(this, EShaderType::PureColor);
}
