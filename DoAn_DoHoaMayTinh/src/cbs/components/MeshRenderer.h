#ifndef MeshRenderer_h
#define MeshRenderer_h

#pragma warning(disable: 26495)

#include "Component.h"
#include "../Object.h"
#include "../connections/PropertyIn.h"
#include "../../scenes/Scene.h"
#include "../../rendering/IDrawable.h"
#include "../../rendering/Model/Model.h"

class Transform;
class MeshRenderer : public Component, public IDrawable {
public:
    MeshRenderer(RawModel& model, EShaderType shader);

    void Initialize() override;
    void Destroy() override;

    void Draw(const ShaderProgram& shader) override;

    PropertyIn<Transform*> TransformIn{ this };

private:
    void DrawMesh(const ShaderProgram& shader, const Mesh& mesh) const;

    Model m_Model;
    EShaderType m_ShaderType;
};

#pragma warning(default: 26495)
#endif
