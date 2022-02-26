#ifndef RawTexture_h
#define RawTexture_h

#include "../debuging/Logger.h"

#pragma warning(push, 0)
#include <iostream>
#include <string>
#pragma warning(pop)

class RawTexture {
public:
    explicit RawTexture(const std::string& path);

    RawTexture() = delete;
    RawTexture(const RawTexture&) = delete;
    RawTexture& operator=(const RawTexture&) = delete;
    RawTexture(RawTexture&&) = delete;
    RawTexture& operator=(RawTexture&&) = delete;
    ~RawTexture();

    unsigned char* Data() const { return m_Data; }
    int Width() const { return m_Width; }
    int Height() const { return m_Height; }
    int Components() const { return m_Components; }

private:
    unsigned char* m_Data;
    int m_Width;
    int m_Height;
    int m_Components;
    std::string m_Path;
};

#endif
