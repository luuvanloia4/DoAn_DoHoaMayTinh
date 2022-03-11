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
int g_BoxTypeCount[10];
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
glm::vec2 g_OffsetX = glm::vec2(-400.0f, -250.0f);
glm::vec2 g_OffsetY = glm::vec2(0.0f, 1000.0f);
glm::vec2 g_OffsetZ = glm::vec2(-250.0, 250.0f);

float g_Gravity = 7.0f;

//Preview
bool g_IsPreview;

class MyScene : public Scene {
public:
	void CreateScene() override;
};

//-------------------------------------------------------------------------------------------------------------
void MyScene::CreateScene() {
    //Game a tri biu
    g_IsPreview = true; // Trước khi bắt đầu chơi, trò chơi sẽ ở chế độ preview
    m_TimeStep = 3.0f;
    m_LastTimeChange = 0.0f;
    m_LimitTime = 300; //5p
    float padding = 20.0f;

    for (int i = 0; i < 10; i++) {
        g_BoxTypeCount[i] = 0;
    }
    //
    LoadMap("map_1.map");

	FrameRateLimit(60);

    //Khởi tạo trạng thái đầu của box map => offset = 0
    m_MapOffset = 0;
    g_MapHeight = 30;
    g_BoxSize = 15.0f;
    g_BoxMargin = 3.0f;
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
    float wallSize = 10.0f;
    glm::vec3 planeColor(0.1f, 0.4f, 0.4f);
    float planeWidth = 2*(g_OffsetZ.y + wallSize);
    float planeHeight = -2*g_OffsetX.x;
    float planeSize = 20.0f;
    glm::mat4 plane_model = glm::mat4(1.0f);
    plane_model = glm::translate(plane_model, vec3(0.0f, -planeSize / 2, 0.0f));
    plane_model = glm::scale(plane_model, glm::vec3(planeHeight, planeSize, planeWidth));
    DrawCuboid(plane_model, planeColor);

    //Tường
    glm::vec3 wallColor(0.2f, 0.2f, 0.3f);
    //--Trước
    float wallWidth = g_MapWidth * (g_BoxSize + g_BoxMargin) - g_BoxMargin;
    float wallHeight = g_MapHeight * (g_BoxSize + g_BoxMargin) - g_BoxMargin + g_BoxMargin;
    g_OffsetY.y = wallHeight; //Giới hạn độ cao là độ cao của boxmap
    glm::mat4 wall_front_model = glm::mat4(1.0f);
    wall_front_model = glm::translate(wall_front_model, glm::vec3(planeHeight / 2, wallHeight / 2, 0.0f));
    wall_front_model = glm::rotate(wall_front_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wall_front_model = glm::scale(wall_front_model, glm::vec3(wallWidth, wallHeight, 0.0f));
    DrawPlane(wall_front_model, wallColor);
    //--Trái
    glm::mat4 wall_left_model = glm::mat4(1.0f);
    wall_left_model = glm::translate(wall_left_model, glm::vec3(0.0f, wallHeight / 2, -planeWidth/2 + wallSize / 2));
    wall_left_model = glm::scale(wall_left_model, glm::vec3(planeHeight, wallHeight, wallSize));
    DrawCuboid(wall_left_model, wallColor);
    ////Phải
    glm::mat4 wall_right_model = glm::mat4(1.0f);
    wall_right_model = glm::translate(wall_right_model, glm::vec3(0.0f, wallHeight / 2, planeWidth / 2 - wallSize / 2));
    wall_right_model = glm::scale(wall_right_model, glm::vec3(planeHeight, wallHeight, wallSize ));
    DrawCuboid(wall_right_model, wallColor);
    //Trên
    glm::mat4 wall_top_model(1.0f);
    wall_top_model = glm::translate(wall_top_model, glm::vec3(0.0f, wallHeight, 0.0f));
    wall_top_model = glm::rotate(wall_top_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    wall_top_model = glm::scale(wall_top_model, glm::vec3(planeHeight, planeWidth, 0.0f));
    DrawPlane(wall_top_model, planeColor);

    //Vạch ngăn:
    //-- Trước
    float limitHeight = 0.7f;
    float limitSize = 10.0f;
    glm::mat4 limit_front_model = glm::mat4(1.0f);
    limit_front_model = glm::translate(limit_front_model, glm::vec3(g_OffsetX.y + limitSize * 2/3, limitHeight / 2, 0.0f));
    limit_front_model = glm::scale(limit_front_model, glm::vec3(limitSize, limitHeight, planeWidth - 2*wallSize));
    DrawCuboid(limit_front_model, glm::vec3(1.0f, 0.3f, 0.2f));

    //-- Sau
    limitHeight = g_CameraOffset.y;
    limitSize = wallSize;
    glm::mat4 limit_back_model = glm::mat4(1.0f);
    limit_back_model = glm::translate(limit_back_model, glm::vec3(g_OffsetX.x + wallSize / 2, limitHeight / 2, 0.0f));
    limit_back_model = glm::scale(limit_back_model, glm::vec3(limitSize, limitHeight, planeWidth - 2 * wallSize));
    DrawCuboid(limit_back_model, wallColor);

    //Game mode object
    float gameModeSize = 25.0f;
    glm::vec3 gameModeOffset(-planeHeight / 2 - gameModeSize * 2 / 3, g_OffsetY.x + gameModeSize * 4 / 5, planeWidth/2 - gameModeSize * 2 /3);

    int gameModeCount = 3;
    glm::vec2 gameModeColorOffset(0.1f, 0.9f);
    float gameModeColorVal = gameModeColorOffset.y - gameModeColorOffset.x;
    gameModeColorVal /= (gameModeCount - 1);

    Bullet** l_BulletArr = new Bullet * [gameModeCount];
    for (int i = 0; i < gameModeCount; i++) {
        l_BulletArr[i] = new Bullet(glm::vec3(gameModeColorOffset.x + i * gameModeColorVal), false, "");
        l_BulletArr[i]->SetSize(glm::vec3(gameModeSize));
        l_BulletArr[i]->MoveTo(gameModeOffset);
        l_BulletArr[i]->m_RollSpeed = (i + 1) * 60;
        PushDrawable(l_BulletArr[i]);
        gameModeOffset.y += 1.5 * gameModeSize;
    }
#pragma endregion

    glm::vec3 startPosition = glm::vec3(planeHeight / 2 - padding, 0.0f, 0.0f);
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
        camera->CreateComponent<Camera>(glm::perspective(glm::radians(45.0f), static_cast<float>(g_Window.Width()) / static_cast<float>(g_Window.Height()), 1.0f, 1500.0f));
        /*auto fpc = camera->CreateComponent<FirstPersonController>();
        camera->Connect(camera->Root().This, fpc->TransformIn);*/

        auto tpc = camera->CreateComponent<ThirdPersonController>(user, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
        camera->Connect(camera->Root().This, tpc->TransformIn);
    }

//Game object
    // -- Box map
    g_OffsetTop = wallHeight - g_BoxSize / 2 - g_BoxMargin;
    g_OffsetBottom = g_BoxSize;
    g_OffsetLeft = g_BoxSize/2 - wallWidth/2;
    g_OffsetFront = planeHeight / 2 - g_BoxSize * 2 / 3;

    //Khởi tạo box map với 2/3 độ dài cột
    g_NumRowLoad = g_MapHeight * 2 / 3;
    int curType;
    for (int i = 0; i < g_NumRowLoad; i++) {
        for (int j = 0; j < g_MapWidth; j++) {
            curType = m_BoxType[g_MapHeight_Max - g_NumRowLoad + i][j];
            g_BoxMap[g_MapHeight - i - 1][j] = new Box(glm::vec3(g_OffsetFront,
                                                g_OffsetTop - i * (g_BoxSize + g_BoxMargin),
                                                g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)), 
                                                curType, false);

            g_BoxMap[g_MapHeight - i - 1][j]->SetSize(glm::vec3(g_BoxSize));
            g_BoxTypeCount[curType]++;
        }
    }

    //Red dot
    g_RedDot = new Bullet(glm::vec3(0.0f), false, "");
    g_RedDot->SetSize(glm::vec3(0.05f));
    g_RedDot->m_IsRedDot = true;
    GenRedDot();
//End Game object
    Preview();//Preview loop

    startPosition.x *= -1;
    user->Root().Position(startPosition);
    g_IsPreview = true; //Đặt flag để chạy vòng lặp trong hàm run()
}