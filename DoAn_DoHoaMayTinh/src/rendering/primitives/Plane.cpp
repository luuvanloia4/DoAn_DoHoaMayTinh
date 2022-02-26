#include "Plane.h"

void Plane::Initialize() {
    float vertices[] = {
        //Position                  //Texture               //Normal
         0.5f,  0.5f, 0.0f,         1.0f, 1.0f,         0.0f, 1.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f,         1.0f, 0.0f,         0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,         0.0f, 0.0f,         0.0f, 1.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,         0.0f, 1.0f,         0.0f, 1.0f, 0.0f, // top left 
         0.5f,  0.5f, 0.0f,         1.0f, 1.0f,         0.0f, 0.0f, 0.0f  // top right
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Plane::Plane(glm::mat4 model, glm::vec3 color)
    : m_Model(model),
    m_Color(color){
    
    Initialize();
}

Plane::Plane(glm::mat4 model, glm::vec3 color, bool haveTexture, std::string texturePath)
    : m_Model(model),
    m_Color(color),
    m_HaveTexture(m_HaveTexture)
{
    if (haveTexture) {
        RawTexture localRawTexture(texturePath);
        Texture localTexture(localRawTexture, (Texture::EType)5);
        m_Texture = localTexture.ID();
    }

    Initialize();
}

Plane::~Plane() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Plane::Draw(const ShaderProgram& shader) {
    shader.Uniform("uniColor", m_Color);
    if (m_HaveTexture) {
        glActiveTexture(GL_TEXTURE_2D);
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
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);
}

void Plane::SetMaterial(Material material) {
    m_Material = material;
}
