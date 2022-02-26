#ifndef Texture_h
#define Texture_h

#include "../resources/RawTexture.h"

#pragma warning(push, 0)
#include <glad/glad.h>
#pragma warning(pop)

class Texture {
public:
    enum class EType {
        Diffuse,
        Specular,
        Ambient,
        Emissive,
        Height,
        Normals,
        Shininess,
        Opacity,
        Displacement,
        Lightmap,
        Reflection,
        Unknown
    };

    Texture(const RawTexture& raw_texture, EType type);
    Texture(const RawTexture& raw_texture, EType type, GLenum wrap_s, GLenum wrap_t, GLenum min_filter, GLenum mag_filter);

    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();

    GLuint ID() const { return m_ID; }
    void Type(EType type) { m_Type = type; }
    EType Type() const { return m_Type; }
    std::string TypeName() const;

private:
    GLuint m_ID;
    EType m_Type;
};

#endif

