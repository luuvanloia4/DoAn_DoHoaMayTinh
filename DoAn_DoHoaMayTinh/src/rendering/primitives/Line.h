#ifndef Line_h
#define Line_h

#include "../IDrawable.h"

class Line : public IDrawable {
public:
    Line(glm::vec3 start, glm::vec3 end, glm::vec3 color);

    Line() = delete;
    Line(const Line&) = delete;
    Line& operator=(const Line&) = delete;
    Line(Line&&) = delete;
    Line& operator=(Line&&) = delete;
    ~Line();

    void Draw(const ShaderProgram& shader) override;

private:
    GLuint m_VBO;
    GLuint m_VAO;
    glm::vec3 m_Color;
};

#endif
