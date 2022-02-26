#include "RawTexture.h"
#include "ResourcesManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

RawTexture::RawTexture(const std::string& path) 
    : m_Path(path) {
    m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Components, 0);
    if (!m_Data) {
        //Logger::Instance().ErrorLog(Logger::ESender::Resources, __FILE__, __LINE__, "Failed to load texture %s", path.c_str());
        std::cout << "Error: fail to load texture " << path << "!" << std::endl;
        m_Data = stbi_load(ERROR_TEXTURE_PATH, &m_Width, &m_Height, &m_Components, 0);
    }
}

RawTexture::~RawTexture() {
    stbi_image_free(m_Data);
}
