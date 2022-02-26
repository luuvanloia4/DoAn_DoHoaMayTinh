#ifndef DrawManager_h
#define DrawManager_h

#include "ShaderProgram.h"
#include "primitives/Cubemap.h"

#pragma warning(push, 0)
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>
#include <dependencies/imgui/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning(pop)

#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <assert.h>

//Material
#include "ListMaterial.h"


class Camera;
class IDrawable;
class IShaderProperty;
class IGUIWidget;

enum class EShaderType {
    PureColor = 0,
    PureTexture,
    Phong,
    Skybox,

    Count
};

class DrawManager {
public:
    void Initialize();

    void RegisterCamera(Camera* camera);
    Camera* MainCamera() const;

    void Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front);
    void Background(const glm::vec3& background);

    void RegisterDrawCall(IDrawable* drawable, EShaderType shader);
    void UnregisterDrawCall(IDrawable* drawable, EShaderType shader);

    void RegisterShaderProperty(const IShaderProperty* property, EShaderType shader);
    void UnregisterShaderProperty(const IShaderProperty* property, EShaderType shader);

    void RegisterGUIWidget(IGUIWidget* widget);
    void UnregisterGUIWidget(IGUIWidget* widget);

    void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
    void DrawPlane(glm::mat4 model, glm::vec3 color);
    void DrawCuboid(glm::mat4 model, glm::vec3 color);
    void DrawCuboid(glm::mat4 model, glm::vec3 color, std::string texturePath);
    void DrawSphere(glm::mat4 model, glm::vec3 color);

    //Game object
    void PushDrawable(IDrawable *obj) {
        m_NextFrameDraws.push_back(obj);
    }
    void PopDrawable(IDrawable* obj) {
        auto to_erase = std::find(m_NextFrameDraws.begin(), m_NextFrameDraws.end(), obj);
        if (to_erase != m_NextFrameDraws.end()) {
            m_NextFrameDraws.erase(to_erase);
        }
    }

    void CallDraws();
    //
    std::array<ShaderProgram, static_cast<size_t>(EShaderType::Count)> m_ShaderPrograms;

private:
    glm::vec3 m_Background{ 0.0f };
    std::unique_ptr<Cubemap> m_Skybox{ nullptr };

    Camera* m_Camera{ nullptr };
    //std::stack<IDrawable*> m_NextFrameDraws;
    std::vector<IDrawable*> m_NextFrameDraws;
    std::vector<IGUIWidget*> m_GUIWidgets;
    ListMaterial m_ListMaterial;
};

#endif
