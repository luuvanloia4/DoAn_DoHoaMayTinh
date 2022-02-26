#ifndef Cube_h
#define Cube_h

#include "Component.h"
#include "../Object.h"
#include "../../scenes/Scene.h"
#include "../connections/PropertyIn.h"
#include "../../rendering/IDrawable.h"

class Cube : public Component, public IDrawable {
public:
    Cube(const glm::vec3& color);

    void Draw(const ShaderProgram& shader) override;

    void Initialize() override;
    void Destroy() override;

    PropertyIn<Transform*> TransformIn{ this };

private:
    GLuint m_VBO;
    GLuint m_VAO;
};


#endif
