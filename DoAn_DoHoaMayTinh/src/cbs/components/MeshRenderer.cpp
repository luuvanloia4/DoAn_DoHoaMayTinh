#include "MeshRenderer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(RawModel& raw_model, EShaderType shader)
    : m_Model(raw_model)
    , m_ShaderType(shader) {
}

void MeshRenderer::Initialize() {
    Object().Scene().RegisterDrawCall(this, m_ShaderType);
}

void MeshRenderer::Destroy() {
    Object().Scene().UnregisterDrawCall(this, m_ShaderType);
}

void MeshRenderer::Draw(const ShaderProgram& shader) {
    shader.Uniform("model", TransformIn.Value()->Model());

    auto end = m_Model.Meshes().end();
    for (auto it = m_Model.Meshes().begin(); it != end; it++) {
        DrawMesh(shader, *it);
    }
}

void MeshRenderer::DrawMesh(const ShaderProgram& shader, const Mesh& mesh) const {
    if (mesh.Diffuse()) {
        glActiveTexture(GL_TEXTURE0);
        shader.Uniform("material.diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, mesh.Diffuse()->ID());
    }

    if (mesh.Specular()) {
        glActiveTexture(GL_TEXTURE1);
        shader.Uniform("material.specular", 1);
        glBindTexture(GL_TEXTURE_2D, mesh.Specular()->ID());
    }

    shader.Uniform("material.shininess", mesh.Shininess());

    glBindVertexArray(mesh.VAO());
    glDrawElements(GL_TRIANGLES, mesh.IndicesCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
