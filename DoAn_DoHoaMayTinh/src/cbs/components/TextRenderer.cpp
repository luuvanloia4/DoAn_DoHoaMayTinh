#include "TextRenderer.h"

TextRenderer::TextRenderer()
    : m_Horizontal(EAlign::NONE)
    , m_Vertical(EAlign::NONE)
    , m_Offset(0.0f)
    , m_Size(300.0f)
    , m_Color(0.0f, 0.0f, 0.0f, 1.0f) {
    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontDefault();
}

TextRenderer::TextRenderer(float size)
    : m_Horizontal(EAlign::NONE)
    , m_Vertical(EAlign::NONE)
    , m_Offset(0.0f)
    , m_Size(300.0f)
    , m_Color(0.0f, 0.0f, 0.0f, 1.0f) {
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = size;
    config.OversampleH = config.OversampleV = 1;
    config.PixelSnapH = true;
    m_Font = io.Fonts->AddFontDefault(&config);
}

TextRenderer::TextRenderer(const std::string& font_path, float size)
    : m_Horizontal(EAlign::NONE)
    , m_Vertical(EAlign::NONE)
    , m_Offset(0.0f)
    , m_Size(300.0f)
    , m_Color(0.0f, 0.0f, 0.0f, 1.0f) {

    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF(font_path.c_str(), size);
    io.Fonts->Build();
}

//void TextRenderer::Initialize() {
//    Object().Scene().RegisterGUIWidget(this);
//}
//
//void TextRenderer::Destroy() {
//    Object().Scene().UnregisterGUIWidget(this);
//}

void TextRenderer::Draw() const {
    ImGui::Begin("dummy", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushFont(m_Font);

    ImVec2 imMargin = ImGui::GetWindowContentRegionMin();
    const glm::vec2 margin = glm::vec2(imMargin.x, imMargin.y);    // Might not be correct
    ImVec2 textSize = ImGui::CalcTextSize(m_Text.c_str());
    const glm::vec2 text_size = glm::vec2(textSize.x, textSize.y);

    glm::vec2 pos(0.0f, 0.0f);
    pos.x = pos.x + m_Offset.x * g_Window.Width();
    pos.y = pos.y + m_Offset.y * g_Window.Height();
    IGUIWidget::Align(&pos.x, -margin.x / 2, g_Window.Width() - margin.x / 2 - text_size.x, m_Horizontal);
    IGUIWidget::Align(&pos.y, -margin.y / 2, g_Window.Height() - margin.y / 2 - text_size.y, m_Vertical);

    ImGui::SetWindowPos(ImVec2(pos.x, pos.y));
    ImGui::TextColored(ImVec4(m_Color.x, m_Color.y, m_Color.z, m_Color.w), m_Text.c_str());

    //ImGui::PopFont();
    ImGui::End();
}

void TextRenderer::Font(const std::string& path, float size) {
    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF(path.c_str(), size);
    io.Fonts->Build();
}

void TextRenderer::Position(glm::vec2 offset, EAlign horizontal, EAlign vertical) {
    m_Offset = offset;
    m_Vertical = vertical;
    m_Horizontal = horizontal;
}
