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

class MyScene : public Scene {
public:
	void CreateScene() override;
    void Preview();
};

//-------------------------------------------------------------------------------------------------------------
void MyScene::CreateScene() {
    //Game a tri biu
    g_IsPreview = true; // Trước khi bắt đầu chơi, trò chơi sẽ ở chế độ preview
    m_TimeStep = 3.0f;
    m_LastTimeChange = 0.0f;
    //
	FrameRateLimit(60);

    //--Read box map type from file
    string mapFolder = "resources/texts/boxmap/";
    string mapPath = mapFolder + "map_1.map";
    
    ifstream iFile(mapPath);
    if (iFile.fail()) {
        std::cout << "ERROR: read map fail: " << mapPath << std::endl;
    }
    else {
        iFile >> g_MapHeight_Max >> g_MapWidth;
        for (int i = 0; i < g_MapHeight_Max; i++) {
            for (int j = 0; j < g_MapWidth; j++) {
                iFile >> m_BoxType[i][j];
            }
        }
    }

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
#pragma region Ngoại cảnh
    //Trụ
    float columnWidth = 80.0f;
    float columnHeight = 400.0f;
    glm::mat4 column_model = glm::mat4(1.0f);
    column_model = glm::translate(column_model, vec3(0.0f, -(columnHeight / 2 + 1), 0.0f));
    column_model = glm::scale(column_model, vec3(columnWidth, columnHeight, columnWidth));
    DrawCuboid(column_model, glm::vec3(0.1f, 0.2f, 0.2f));

    //Nền
    float planeWidth = 400.0f;
    float planeHeight = 400.0f;
    float planeSize = 10.0f;
    glm::mat4 plane_model = glm::mat4(1.0f);
    plane_model = glm::translate(plane_model, vec3(0.0f, -planeSize / 2, 0.0f));
    plane_model = glm::scale(plane_model, glm::vec3(planeWidth, planeSize, planeHeight));
    DrawCuboid(plane_model, glm::vec3(0.1f, 0.4f, 0.4f));

    //Tường
    float wallWidth = g_MapWidth * (g_BoxSize + g_BoxMargin) - g_BoxMargin;
    float wallHeight = g_MapHeight * (g_BoxSize + g_BoxMargin) - g_BoxMargin;
    glm::mat4 wall_model = glm::mat4(1.0f);
    wall_model = glm::translate(wall_model, glm::vec3(planeWidth / 2, wallHeight / 2, 0.0f));
    wall_model = glm::rotate(wall_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wall_model = glm::scale(wall_model, glm::vec3(wallWidth, wallHeight, 0.0f));
    DrawPlane(wall_model, glm::vec3(0.2f, 0.2f, 0.3f));

    //Vách ngăn:
    float limitHeight = 0.3f;
    float limitWidth = 10.0f;
    glm::mat4 limit_model = glm::mat4(1.0f);
    limit_model = glm::translate(limit_model, glm::vec3(g_OffsetX.y + limitWidth / 2, limitHeight / 2, 0.0f));
    limit_model = glm::scale(limit_model, glm::vec3(limitWidth, limitHeight, planeWidth));
    DrawCuboid(limit_model, glm::vec3(1.0f, 0.3f, 0.2f));

    //Sample object
    glm::mat4 cuboid_model = glm::mat4(1.0f);
    cuboid_model = glm::translate(cuboid_model, glm::vec3(-20.0f, g_BoxSize / 2, 0.0f));
    cuboid_model = glm::scale(cuboid_model, glm::vec3(g_BoxSize / 2));
    DrawCuboid(cuboid_model, vec3(0.0f, 0.9f, 0.7f), "resources/textures/box5.jfif");
#pragma endregion

    //User
    auto user = CreateObject("user"); {
        user->Root().Move(glm::vec3(planeWidth/2, 0.0f, 0.0f));

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

//Game object
    // -- Box map
    g_OffsetTop = wallHeight - g_BoxSize / 2;
    g_OffsetBottom = g_BoxSize;
    g_OffsetLeft = g_BoxSize/2 - wallWidth/2;
    g_OffsetFront = planeHeight / 2 - g_BoxSize * 2 / 3;

    //Khởi tạo box map với 2/3 độ dài cột
    g_NumRowLoad = g_MapHeight * 2 / 3;
    for (int i = 0; i < g_NumRowLoad; i++) {
        for (int j = 0; j < g_MapWidth; j++) {
            g_BoxMap[g_MapHeight - i - 1][j] = new Box(glm::vec3(g_OffsetFront,
                                                g_OffsetTop - i * (g_BoxSize + g_BoxMargin),
                                                g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)), 
                                                m_BoxType[g_MapHeight_Max - g_NumRowLoad + i][j], false);

            g_BoxMap[g_MapHeight - i - 1][j]->SetSize(glm::vec3(g_BoxSize));
        }
    }

    //Red dot
    g_RedDot = new Bullet(glm::vec3(0.0f), false, "");
    int newBulletType = rand() % 9 + 1;
    g_RedDot->m_Type = newBulletType;
    g_RedDot->m_Color = Box::GenColor(newBulletType);
    g_RedDot->SetSize(glm::vec3(0.05f));
//End Game object
}

//-------------------------------------------------------------------------------------------------------------
void MyScene::Preview() {

    // Initialize Time manager as close to game loop as possible
    // to avoid misrepresented delta time
    g_Time.Initialize();

    m_TxtMessage = new TextRenderer("resources/fonts/times.ttf", 25.0f);
    m_TxtMessage->Text("Press F to Start the game!!!");
    m_TxtMessage->Position(glm::vec2(0.0f), TextRenderer::EAlign::CENTER, TextRenderer::EAlign::CENTER);
    m_TxtMessage->Color(glm::vec4(1.0f));
    int timeCount = 0;
    m_DrawManager.RegisterGUIWidget(m_TxtMessage);
    bool isShowText = true;
    int flashSpeed = 2;

    while (g_IsPreview) {
        // If frame rate is greater than limit then wait
        do {
            g_Time.Hold();
            glfwPollEvents();
        } while (g_Time.DeltaTime() < m_FrameRateLimit);

        // Update global systems
        g_Time.Update();
        g_Input.Update(g_Window);

        timeCount = g_Time.CurrentTime();
        if (timeCount % flashSpeed == flashSpeed - 1) {
            if (isShowText && timeCount + 0.5f > g_Time.CurrentTime()) {
                m_DrawManager.UnregisterGUIWidget(m_TxtMessage);
                isShowText = false;
            }
            else if(!isShowText && timeCount + 0.5f <= g_Time.CurrentTime()){
                m_DrawManager.RegisterGUIWidget(m_TxtMessage);
                isShowText = true;
            }
        }

        //Draw
        // Update managers
        //m_PhysicsManager.StepSimulation(g_Time.DeltaTime());
        m_ObjectManager.ProcessFrame();
        m_DrawManager.CallDraws();
        //g_IsPreview = false;
    }
    
    //End preview
    m_DrawManager.UnregisterGUIWidget(m_TxtMessage);
};