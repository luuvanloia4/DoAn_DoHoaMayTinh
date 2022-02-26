#ifndef ShaderProgram_h
#define ShaderProgram_h

#include "../debuging/Logger.h"
#include "../core/Enum.h"

#pragma warning(push, 0)
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#pragma warning(pop)

class IDrawable;
class IShaderProperty;

class ShaderProgram {
public:
    enum class ETrait : unsigned char {
        NONE = 0,
        LIGHT_RECEIVER = 1 << 0
    };

    ShaderProgram();
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&&) = default;
    ShaderProgram& operator=(ShaderProgram&&) = default;
    ~ShaderProgram();
    
    void AttachShaders(const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);
    void Use() const;

    GLuint ID() const { return m_ID; }

    ETrait Traits() const { return m_Traits; }
    void Traits(ETrait traits) { m_Traits = traits; }

    void RegisterDrawCall(IDrawable* drawable);
    void UnregisterDrawCall(IDrawable* drawable);

    void RegisterShaderProperty(const IShaderProperty* property);
    void UnregisterShaderProperty(const IShaderProperty* property);

    void CallProperties() const;
    void CallDraws();

    // Setters for OpenGL shaders
    void Uniform(const std::string &name, bool value) const;
    void Uniform(const std::string &name, int value) const;
    void Uniform(const std::string &name, float value) const;
    void Uniform(const std::string &name, const glm::vec2 &vec) const;
    void Uniform(const std::string &name, float x, float y) const;
    void Uniform(const std::string &name, const glm::vec3 &vec) const;
    void Uniform(const std::string &name, float x, float y, float z) const;
    void Uniform(const std::string &name, const glm::vec4 &vec) const;
    void Uniform(const std::string &name, float x, float y, float z, float w) const;
    void Uniform(const std::string &name, const glm::mat2 &mat) const;
    void Uniform(const std::string &name, const glm::mat3 &mat) const;
    void Uniform(const std::string &name, const glm::mat4 &mat) const;

private:
    void LinkProgram();
    unsigned int AttachShader(const char *path, GLenum shader);

    GLuint m_ID;
    ETrait m_Traits;
    std::vector<IDrawable*> m_Drawables;
    std::vector<const IShaderProperty*> m_Properties;
};

ENABLE_BITMASK_OPERATORS(ShaderProgram::ETrait);

#endif
