#include "Texture.h"

Texture::Texture(const RawTexture& raw_texture, Texture::EType type = Texture::EType::Diffuse)
    : m_Type(type) {
    const GLenum format = [&]() {
        switch (raw_texture.Components()) {
        case 1:
            return GL_RED;

        case 3:
            return GL_RGB;

        case 4:
            return GL_RGBA;

        default:
            return GL_RGB;
        }
    }();

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, raw_texture.Width(), raw_texture.Height(), 0, format, GL_UNSIGNED_BYTE, raw_texture.Data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const RawTexture& raw_texture, EType type, GLenum wrap_s, GLenum wrap_t, GLenum min_filter, GLenum mag_filter)
    : m_Type(type) {
    const GLenum format = [&]() {
        switch (raw_texture.Components()) {
        case 1:
            return GL_RED;

        case 3:
            return GL_RGB;

        case 4:
            return GL_RGBA;

        default:
            return -1;
        }
    }();

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    glTexImage2D(GL_TEXTURE_2D, 0, format, raw_texture.Width(), raw_texture.Height(), 0, format, GL_UNSIGNED_BYTE, raw_texture.Data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(Texture&& other) noexcept
    : m_ID(std::exchange(other.m_ID, 0))
    , m_Type(other.m_Type) {
}

Texture& Texture::operator=(Texture&& other) noexcept {
    m_ID = std::exchange(other.m_ID, 0);
    m_Type = other.m_Type;

    return *this;
}

Texture::~Texture() {
    glDeleteTextures(1, &m_ID);
}

std::string Texture::TypeName() const {
    switch (m_Type) {
    case Texture::EType::Diffuse:
        return "diffuse";

    case Texture::EType::Specular:
        return "specular";

    case Texture::EType::Ambient:
        return "ambient";

    case Texture::EType::Emissive:
        return "emissive";

    case Texture::EType::Height:
        return "height";

    case Texture::EType::Normals:
        return "normals";

    case Texture::EType::Shininess:
        return "shininess";

    case Texture::EType::Opacity:
        return "opacity";

    case Texture::EType::Displacement:
        return "displacement";

    case Texture::EType::Lightmap:
        return "lightmap";

    case Texture::EType::Reflection:
        return "reflection";

    default: // RawTexture::EType::Unknown
        return "unknown";
    }
}