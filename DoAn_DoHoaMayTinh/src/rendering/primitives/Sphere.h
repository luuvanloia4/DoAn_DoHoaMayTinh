#ifndef Sphere_h
#define Sphere_h

#include "../IDrawable.h"

class Sphere : public IDrawable {
public:
    Sphere(glm::mat4 model, glm::vec3 color);

    Sphere() = delete;
    Sphere(const Sphere&) = delete;
    Sphere& operator=(const Sphere&) = delete;
    Sphere(Sphere&&) = delete;
    Sphere& operator=(Sphere&&) = delete;
    ~Sphere();

    void Draw(const ShaderProgram& shader) override;

private:
    GLuint m_VBO;
    GLuint m_CBO; // ColorBufferObject
    GLuint m_VAO;
    glm::mat4 m_Model;
};

#endif