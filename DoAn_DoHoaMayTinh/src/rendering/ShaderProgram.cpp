#include "ShaderProgram.h"

#include "IDrawable.h"
#include "IShaderProperty.h"

ShaderProgram::ShaderProgram()
    : m_Traits(ETrait::NONE) {
    m_ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_ID);
}

void ShaderProgram::Use() const {
    glUseProgram(m_ID);
}

void ShaderProgram::AttachShaders(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
    // Compile shaders from given files
    unsigned int vertex_shader = AttachShader(vertex_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader = AttachShader(fragment_path, GL_FRAGMENT_SHADER);
    unsigned int geometry_shader = geometry_path != nullptr ? AttachShader(geometry_path, GL_GEOMETRY_SHADER) : -1;

    LinkProgram();

    // Free memory
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if(geometry_path != nullptr) {
        glDeleteShader(geometry_shader);
    }
}

void ShaderProgram::RegisterDrawCall(IDrawable* drawable) {
    // Ensure that each component is registered at most once
    assert(std::find(m_Drawables.begin(), m_Drawables.end(), drawable) == m_Drawables.end());

    m_Drawables.push_back(drawable);
}

void ShaderProgram::UnregisterDrawCall(IDrawable* drawable) {
    // Unregistering not registered component has no effect
    auto to_erase = std::find(m_Drawables.begin(), m_Drawables.end(), drawable);
    if (to_erase != m_Drawables.end()) {
        m_Drawables.erase(to_erase);
    }
}

void ShaderProgram::RegisterShaderProperty(const IShaderProperty* property) {
    // Ensure that each component is registered at most once
    assert(std::find(m_Properties.begin(), m_Properties.end(), property) == m_Properties.end());

    m_Properties.push_back(property);
}

void ShaderProgram::UnregisterShaderProperty(const IShaderProperty* property) {
    // Unregistering not registered component has no effect
    auto to_erase = std::find(m_Properties.begin(), m_Properties.end(), property);
    if (to_erase != m_Properties.end()) {
        m_Properties.erase(to_erase);
    }
}

void ShaderProgram::CallProperties() const {
    for (auto& property : m_Properties) {
        property->SetProperty(*this);
    }
}

void ShaderProgram::CallDraws() {
    for (auto& drawable : m_Drawables) {
        drawable->Draw(*this);
    }
}

void ShaderProgram::Uniform(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void ShaderProgram::Uniform(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::Uniform(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::Uniform(const std::string &name, const glm::vec2 &vec) const {
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, value_ptr(vec));
}

void ShaderProgram::Uniform(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void ShaderProgram::Uniform(const std::string &name, const glm::vec3 &vec) const {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, value_ptr(vec));
}

void ShaderProgram::Uniform(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void ShaderProgram::Uniform(const std::string &name, const glm::vec4 &vec) const {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, value_ptr(vec));
}

void ShaderProgram::Uniform(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void ShaderProgram::Uniform(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}

void ShaderProgram::Uniform(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}

void ShaderProgram::Uniform(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}

unsigned int ShaderProgram::AttachShader(const char *path, GLenum shader_type) {
    std::string shader_code;
    std::fstream shader_file;
    
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // Read file
    try {
        shader_file.open(path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        
        shader_code = shader_stream.str();
    } catch(const std::ifstream::failure &e) {
        ERROR_LOG(Logger::ESender::Rendering, "Failed to read shader file %s:\n%s", path, e.what());
    }

    // Compile shader
    unsigned int shader = glCreateShader(shader_type);
    const char *shader_code_ptr = shader_code.c_str();
    glShaderSource(shader, 1, &shader_code_ptr, nullptr);
    glCompileShader(shader);
    
    // Check compile errors
    GLint success;
    GLchar info_log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        ERROR_LOG(Logger::ESender::Rendering, "Failed to compile shader %s:\n%s", path, info_log);
    }
    
    glAttachShader(m_ID, shader);
    
    return shader;
}

void ShaderProgram::LinkProgram() {
    glLinkProgram(m_ID);
    
    // Check linking errors
    int success;
    char info_log[1024];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 1024, nullptr, info_log);
        Logger::Instance().ErrorLog(Logger::ESender::Rendering, "Failed to link shader %d:\n%s", m_ID, info_log);
    }
}
