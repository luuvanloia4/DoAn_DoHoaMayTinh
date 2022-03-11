#include "DrawManager.h"

#include "IDrawable.h"
#include "IShaderProperty.h"
#include "IGUIWidget.h"
#include "primitives/Line.h"
#include "primitives/Plane.h"
#include "primitives/Cuboid.h"
#include "primitives/Cubemap.h"
#include "../utilities/Window.h"
#include "../rendering/primitives/Cubemap.h"
#include "../cbs/components/Camera.h"

void DrawManager::Initialize() {
    INFO_LOG(Logger::ESender::Rendering, "Initializing draw manager");

    // Dear imgui initialiation
    IMGUI_CHECKVERSION();
    ImGui::SetCurrentContext(ImGui::CreateContext());
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // Load default font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Create shader programs
    m_ShaderPrograms[static_cast<size_t>(EShaderType::PureColor)].AttachShaders("src/shaders/PURE_COLOR.vert",
                                                                                "src/shaders/PURE_COLOR.frag");


    m_ShaderPrograms[static_cast<size_t>(EShaderType::PureTexture)].AttachShaders("src/shaders/PURE_TEXTURE.vert",
                                                                                  "src/shaders/PURE_TEXTURE.frag");


    m_ShaderPrograms[static_cast<size_t>(EShaderType::Phong)].AttachShaders("src/shaders/PHONG.vert",
                                                                            "src/shaders/PHONG.frag");
    m_ShaderPrograms[static_cast<size_t>(EShaderType::Phong)].Traits(ShaderProgram::ETrait::LIGHT_RECEIVER);


    m_ShaderPrograms[static_cast<size_t>(EShaderType::Skybox)].AttachShaders("src/shaders/SKYBOX.vert",
                                                                             "src/shaders/SKYBOX.frag");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void DrawManager::RegisterCamera(Camera *camera) {
    m_Camera = camera;
}

Camera* DrawManager::MainCamera() const {
    return m_Camera;
}

void DrawManager::Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front) {
    m_Skybox = std::make_unique<Cubemap>(right, left, top, bottom, back, front);
}

void DrawManager::Background(const glm::vec3& background) {
    m_Background = background;
}

void DrawManager::RegisterDrawCall(IDrawable* drawable, EShaderType shader) {
    m_ShaderPrograms[static_cast<size_t>(shader)].RegisterDrawCall(drawable);
}

void DrawManager::UnregisterDrawCall(IDrawable* drawable, EShaderType shader) {
    m_ShaderPrograms[static_cast<size_t>(shader)].UnregisterDrawCall(drawable);
}

void DrawManager::RegisterShaderProperty(const IShaderProperty* property, EShaderType shader) {
    m_ShaderPrograms[static_cast<size_t>(shader)].RegisterShaderProperty(property);
}

void DrawManager::UnregisterShaderProperty(const IShaderProperty* property, EShaderType shader) {
    m_ShaderPrograms[static_cast<size_t>(shader)].UnregisterShaderProperty(property);
}

void DrawManager::RegisterGUIWidget(IGUIWidget* widget) {
    // Ensure that each widget is registered at most once
    assert(std::find(m_GUIWidgets.begin(), m_GUIWidgets.end(), widget) == m_GUIWidgets.end());

    m_GUIWidgets.push_back(widget);
}

void DrawManager::UnregisterGUIWidget(IGUIWidget* widget) {
    // Unregistering not registered widget has no effect
    auto to_erase = std::find(m_GUIWidgets.begin(), m_GUIWidgets.end(), widget);
    if (to_erase != m_GUIWidgets.end()) {
        m_GUIWidgets.erase(to_erase);
    }
}

// TODO optimise 
void DrawManager::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    auto lineObject = new Line(start, end, color);
    m_NextFrameDraws.push_back(lineObject);
}

// TODO optimise
void DrawManager::DrawPlane(glm::mat4 model, glm::vec3 color) {
    auto panelObject = new Plane(model, color);
    panelObject->SetMaterial(m_ListMaterial.Chrome);
    m_NextFrameDraws.push_back(panelObject);
}

// TODO optimise
void DrawManager::DrawCuboid(glm::mat4 model, glm::vec3 color) {
    auto cuboidObject = new Cuboid(model, color, false, "", false, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    cuboidObject->SetMaterial(m_ListMaterial.Silver);
    m_NextFrameDraws.push_back(cuboidObject);
}

void DrawManager::DrawCuboid(glm::mat4 model, glm::vec3 color, std::string texturePath) {
    auto cuboidObject = new Cuboid(model, color, true, texturePath, true, glm::vec3(0.0f, 1.0f, 0.0f), 10.0f);
    cuboidObject->SetMaterial(m_ListMaterial.Silver);
    m_NextFrameDraws.push_back(cuboidObject);
}

// TODO optimise
void DrawManager::DrawSphere(glm::mat4 model, glm::vec3 color) {

}

void DrawManager::CallDraws() {
    glClearColor(m_Background.x, m_Background.y, m_Background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 pv = m_Camera->Projection() * m_Camera->ViewMatrix();
    glm::vec3 cameraPos = m_Camera->m_Transform->Position() + g_CameraOffset;
    glm::vec3 cameraFront = m_Camera->m_Transform->Front();
    g_RedDot->UpdatePosition(cameraPos, cameraFront);

    // Call draws in all shaders
    for (auto shader = m_ShaderPrograms.begin(); shader != m_ShaderPrograms.end(); shader++) {
        shader->Use();

        shader->Uniform("pv", pv);
        shader->Uniform("viewPos", m_Camera->Object().Root().Position());

        shader->CallProperties();
        shader->CallDraws();
    }

    // Call one-frame draws
    //Color
    auto& pure_color_shader = m_ShaderPrograms[static_cast<size_t>(EShaderType::PureColor)];
    pure_color_shader.Use();

    if (!g_IsPreview) {
        if (g_RedDotSize >= 0.04f) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        g_RedDot->Draw(pure_color_shader);
    }
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (auto bullet : g_BulletQueue) {
        bullet->Draw(pure_color_shader);
    }

    for (int i = 0; i < g_MapHeight; i++) {
        for (int j = 0; j < g_MapWidth; j++) {
            if (g_BoxMap[i][j] != NULL) {
                g_BoxMap[i][j]->Draw(pure_color_shader);
            }
        }
    }

    for (auto item : m_NextFrameDraws) {
        //glBindTexture(GL_TEXTURE_2D, 0);
        item->Draw(pure_color_shader);
    }

    //Texture
    auto& pure_texture_shader = m_ShaderPrograms[static_cast<size_t>(EShaderType::PureTexture)];
    pure_texture_shader.Use();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Draw skybox
    if (m_Skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        const ShaderProgram& skybox_shader = m_ShaderPrograms[static_cast<size_t>(EShaderType::Skybox)];

        skybox_shader.Use();
        skybox_shader.Uniform("pv", m_Camera->Projection() * glm::mat4(glm::mat3(m_Camera->ViewMatrix())));

        m_Skybox->Draw(skybox_shader);

        glDepthFunc(GL_LESS);
    }

    // Draw GUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto widget : m_GUIWidgets) {
        widget->Draw();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();

    // End of drawing
    glfwSwapBuffers(g_Window);
}
