#include "Scene.h"

#include "../cbs/components/MeshRenderer.h"
#include "../cbs/components/Camera.h"
#include "../cbs/components/FirstPersonController.h"
#include "../cbs/components/ThirdPersonController.h"
#include "../cbs/components/DirectionalLight.h"
//#include "../cbs/components/World/World.h"

#include "../gameobject/Box.h"
#include "../gameobject/Bullet.h"

#include <fstream>

#pragma once
//Score
int g_Score = 0;

//Bullet
int g_BulletCount = 0;
std::deque<Bullet*> g_BulletQueue;
Bullet* g_RedDot;
float g_RedDotSize = 0.05f;
bool g_IsFire = false;

//Box map
Box* g_BoxMap[31][20];
int g_MapWidth;
int g_MapHeight;
int g_MapHeight_Max;
int g_NumRowLoad;
float g_OffsetLeft;
float g_OffsetTop;
float g_OffsetBottom;
float g_OffsetFront;
//Box
float g_BoxSize;
float g_BoxMargin;

//Camera
glm::vec3 g_CameraOffset = glm::vec3(0.0f, 50.0f, 0.0f);
glm::quat g_CameraRotation;

//Time

//And

//Relative Dimension In Space
glm::vec2 g_OffsetX = glm::vec2(-200.0f, -100.0f);
glm::vec2 g_OffsetY = glm::vec2(0.0f, 1000.0f);
glm::vec2 g_OffsetZ = glm::vec2(-200.0, 200.0f);

float g_Gravity = 5.0f;

//Preview
bool g_IsPreview;


class TestScene : public Scene {
public:
    void CreateScene() override;
};

//-------------------------------------------------------------------------------------------------------------
void TestScene::CreateScene() {
    //Game a tri biu
    g_IsPreview = true; // Trước khi bắt đầu chơi, trò chơi sẽ ở chế độ preview
    m_TimeStep = 3.0f;
    m_LastTimeChange = 0.0f;
    //
    FrameRateLimit(60);
    g_MapWidth = 0;
    g_MapHeight_Max = 0;

    //Khởi tạo trạng thái đầu của box map => offset = 0
    m_MapOffset = 0;
    g_MapHeight = 30;
    g_BoxSize = 8.0f;
    g_BoxMargin = 1.5f;
    //mars
    string skyboxName = "mars";
    skyboxName += "/";

    Skybox("resources/skyboxes/" + skyboxName + "right.png",
        "resources/skyboxes/" + skyboxName + "left.png",
        "resources/skyboxes/" + skyboxName + "top.png",
        "resources/skyboxes/" + skyboxName + "bottom.png",
        "resources/skyboxes/" + skyboxName + "back.png",
        "resources/skyboxes/" + skyboxName + "front.png");

    //Background(glm::vec3(0.0f, 100.0f, 200.0f));

    auto light = CreateObject("Light"); {
        light->CreateComponent<DirectionalLight>(glm::vec3(1.0f, 0.5f, 0.4f),
            glm::vec3(0.8f),
            glm::vec3(0.7f, 0.68f, 0.68f),
            glm::vec3(1.0f, 1.0f, 1.0f));
    }

    glm::vec3 startPosition = glm::vec3(10.0f / 2, 0.0f, 0.0f);
    //User
    auto user = CreateObject("user"); {
        user->Root().Move(startPosition);

        //User
        auto mesh_transform = user->CreateComponent<Transform>();
        user->Connect(user->Root().This, mesh_transform->Parent);
        mesh_transform->Position(glm::vec3(0.0f, 0.005f, 0.0f));
        mesh_transform->Scale(glm::vec3(5.0f));

        /*auto user_mesh = user->CreateComponent<MeshRenderer>(LoadModel("resources/models/user/user.obj"), EShaderType::Phong);
        user->Connect(mesh_transform->This, user_mesh->TransformIn);*/

        //auto camera = user1->CreateComponent<Camera>(glm::perspective(glm::radians(45.0f), static_cast<float>(g_Window.Width()) / static_cast<float>(g_Window.Height()), 0.1f, 500.0f));
        auto fpc = user->CreateComponent<FirstPersonController>(200.0f, 80.0f, 0.1f);
        user->Connect(user->Root().This, fpc->TransformIn);
        /*auto tpc = user->CreateComponent<ThirdPersonController>(user, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
        user->Connect(user->Root().This, tpc->TransformIn);*/
    }

    auto camera = CreateObject("Camera"); {
        //camera->Root().Move(glm::vec3(3.0f, 0.0f, 2.0f));
        camera->CreateComponent<Camera>(glm::perspective(glm::radians(45.0f), static_cast<float>(g_Window.Width()) / static_cast<float>(g_Window.Height()), 1.0f, 800.0f));
        /*auto fpc = camera->CreateComponent<FirstPersonController>();
        camera->Connect(camera->Root().This, fpc->TransformIn);*/

        auto tpc = camera->CreateComponent<ThirdPersonController>(user, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
        camera->Connect(camera->Root().This, tpc->TransformIn);
    }
}

