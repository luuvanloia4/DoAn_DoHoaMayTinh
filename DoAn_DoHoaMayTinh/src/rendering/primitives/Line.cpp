#include "Line.h"

Line::Line(glm::vec3 start, glm::vec3 end, glm::vec3 color)
    : m_Color(color) {
    GLfloat vertices[] = { 
        start.x, start.y, start.z, color.x, color.y, color.z,
        end.x,   end.y,   end.z,   color.x, color.y, color.z
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

Line::~Line() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Line::Draw(const ShaderProgram &shader) {
    shader.Uniform("model", glm::mat4(1.0f));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
