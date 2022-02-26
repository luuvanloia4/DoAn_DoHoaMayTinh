#ifndef TextRenderer_h
#define TextRenderer_h

#include "Component.h"
#include "../../rendering/IGUIWidget.h"
#include "../Object.h"
#include "../../scenes/Scene.h"
#include "../connections/MessageIn.h"

#include <string>

class TextRenderer : public Component, public IGUIWidget {
public:
    TextRenderer(const std::string& font_path, float size);

    void Initialize() override;
    void Destroy() override;

    void Draw() const override;

    void Font(const std::string& path, float size);
    void Position(glm::vec2 offset, EAlign horizontal, EAlign vertical);

    const std::string Text() const { return m_Text; }
    void Text(std::string text) { m_Text = text; }

    const glm::vec4 Color() const { return m_Color; }
    void Color(glm::vec4 color) { m_Color = color; }

public:
    MessageIn<std::string, TextRenderer, &TextRenderer::Text> TextIn{ this };
    MessageIn<glm::vec4, TextRenderer, &TextRenderer::Color> ColorIn{ this };

private:
    std::string m_Text;

    EAlign m_Horizontal;
    EAlign m_Vertical;
    glm::vec2 m_Offset;
    glm::vec4 m_Color;
    ImFont* m_Font;
};

#endif