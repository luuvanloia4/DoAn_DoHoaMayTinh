#include "Scene.h"

//Scene::Scene() 
//    : m_ObjectManager(*this)
//    , m_PhysicsManager(new PhysicsRenderer(m_DrawManager)) {
//}

//Thuat toan
void TryToKill(int posY, int posX, int typeToKill, int& killCount) {
    if (g_BoxMap[posY][posX] != NULL && g_BoxMap[posY][posX]->m_Type == typeToKill) {
        killCount++;
        delete g_BoxMap[posY][posX];
        g_BoxMap[posY][posX] = NULL;

        //Loang
        if (posY > 0 && g_BoxMap[posY - 1][posX] != NULL) {
            TryToKill(posY - 1, posX, typeToKill, killCount);
        }
        if (posY < g_MapHeight - 1 && g_BoxMap[posY + 1][posX] != NULL) {
            TryToKill(posY + 1, posX, typeToKill, killCount);
        }
        if (posX > 0 && g_BoxMap[posY][posX - 1] != NULL) {
            TryToKill(posY, posX - 1, typeToKill, killCount);
        }
        if (posX < g_MapWidth - 1 && g_BoxMap[posY][posX + 1] != NULL) {
            TryToKill(posY, posX + 1, typeToKill, killCount);
        }
    }
}

std::string ToTime(int second) {
    int hour = second / 3600;
    second %= 3600;
    int minute = second / 60;
    second %= 60;

    return std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
}

Scene::Scene()
    : m_ObjectManager(*this) {
}

void Scene::Initialize() {
    m_DrawManager.Initialize();
    //m_PhysicsManager.Initialize();
    //m_AudioManager.Initialize();
}

void Scene::GenRedDot() {
    int newType;
    int typeIndex = 0;
    int l_BoxType[10];
    for (int i = 0; i < 10; i++) {
        if (g_BoxTypeCount[i] > 0) {
            l_BoxType[typeIndex++] = i;
        }
    }
    newType = rand() % typeIndex;
    newType = l_BoxType[newType];
    
    g_RedDot->m_Type = newType;
    g_RedDot->m_Color = Box::GenColor(newType);
    g_RedDotSize = 0.05f;
}

void Scene::LoadMap(string mapName) {
    //--Read box map type from file
    string mapFolder = "resources/texts/boxmap/";
    string mapPath = mapFolder + mapName;

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
}

void Scene::CheckKeypress() {
    if (g_Input.KeyReleased(GLFW_KEY_ESCAPE)) {
        int endSelected = MessageBox(NULL, TEXT("Bạn có thực sự muốn THOÁT khỏi trò chơi?"), TEXT("Thông báo:"), MB_ICONINFORMATION | MB_OKCANCEL);
        if (endSelected == IDOK) {
            g_IsPreview = false;
            Exit();
        }
        else {
            //
        }
    }

    if (g_Input.KeyHold(GLFW_KEY_LEFT_CONTROL) || g_Input.KeyHold(GLFW_KEY_RIGHT_CONTROL)) {
        if (g_Input.KeyPressed(GLFW_KEY_1) && m_MapCode != 1) {
            int endSelected = MessageBox(NULL, TEXT("Bạn có thực sự muốn đổi sang MAP 1?"), TEXT("Đổi map:"), MB_ICONINFORMATION | MB_OKCANCEL);
            if (endSelected == IDOK) {
                m_MapCode = 1;
                m_IsChangeMap = true;
                ResetGame();
            }
            else {
                //
            }
        }
        else if (g_Input.KeyPressed(GLFW_KEY_2) && m_MapCode != 2) {
            int endSelected = MessageBox(NULL, TEXT("Bạn có thực sự muốn đổi sang MAP 2?"), TEXT("Đổi map:"), MB_ICONINFORMATION | MB_OKCANCEL);
            if (endSelected == IDOK) {
                m_MapCode = 2;
                m_IsChangeMap = true;
                ResetGame();
            }
            else {
                //
            }
        }
        else if (g_Input.KeyPressed(GLFW_KEY_3) && m_MapCode != 3) {
            int endSelected = MessageBox(NULL, TEXT("Bạn có thực sự muốn đổi sang MAP 3?"), TEXT("Đổi map:"), MB_ICONINFORMATION | MB_OKCANCEL);
            if (endSelected == IDOK) {
                m_MapCode = 3;
                m_IsChangeMap = true;
                ResetGame();
            }
            else {
                //
            }
        }
    }

    if (!g_IsPreview && g_Input.KeyReleased(GLFW_KEY_F5)) {
        int endSelected = MessageBox(NULL, TEXT("Bạn có thực sự muốn LÀM LẠI?"), TEXT("Thông báo:"), MB_ICONINFORMATION | MB_OKCANCEL);
        if (endSelected == IDOK) {
            ResetGame();
        }
        else {
            //
        }
    }

    if (g_Input.KeyReleased(GLFW_KEY_F11)) {
        g_Window.SwapScreenSize();
    }
}

void Scene::Preview() {
    m_Running = true;
    // Initialize Time manager as close to game loop as possible
    // to avoid misrepresented delta time
    g_Time.Initialize();

    m_TxtMessage = new TextRenderer("resources/fonts/times.ttf", 25.0f);
    m_TxtMessage->Text("This is Preview mode, PRESS SPACE to START the game!!!");
    m_TxtMessage->Position(glm::vec2(0.0f), TextRenderer::EAlign::CENTER, TextRenderer::EAlign::CENTER);
    m_TxtMessage->Color(glm::vec4(1.0f));
    int timeCount = 0;
    m_DrawManager.RegisterGUIWidget(m_TxtMessage);
    bool isShowText = true;
    int flashSpeed = 1;

    while (g_IsPreview && !glfwWindowShouldClose(g_Window)) {
        // If frame rate is greater than limit then wait
        do {
            g_Time.Hold();
            glfwPollEvents();
        } while (g_Time.DeltaTime() < m_FrameRateLimit);

        // Update global systems
        g_Time.Update();
        g_Input.Update(g_Window);

        if (m_IsReset) {
            g_Score = 0;
            for (int i = 0; i < 10; i++) {
                g_BoxTypeCount[i] = 0;
            }

            if (m_IsChangeMap) {
                switch (m_MapCode)
                {
                case 1:
                    LoadMap("map_1.map");
                    break;
                case 2:
                    LoadMap("map_2.map");
                    break;
                case 3:
                    LoadMap("map_3.map");
                    break;
                default:
                    break;
                }

                //
                m_IsChangeMap = false;
            }

            m_MapOffset = 0;
            int curType;
            for (int i = 0; i < g_MapHeight; i++) {
                for (int j = 0; j < g_MapWidth; j++) {
                    if (g_BoxMap[g_MapHeight - i - 1][j] != NULL) {
                        delete g_BoxMap[g_MapHeight - i - 1][j];
                        g_BoxMap[g_MapHeight - i - 1][j] = NULL;
                    }
                    if (i < g_NumRowLoad) {
                        curType = m_BoxType[g_MapHeight_Max - g_NumRowLoad + i][j];
                        g_BoxMap[g_MapHeight - i - 1][j] = new Box(glm::vec3(g_OffsetFront,
                            g_OffsetTop - i * (g_BoxSize + g_BoxMargin),
                            g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)),
                            curType, false);

                        g_BoxMap[g_MapHeight - i - 1][j]->SetSize(glm::vec3(g_BoxSize));
                        g_BoxTypeCount[curType]++;
                    }
                }
            }

            GenRedDot();
            //
            m_IsReset = false;
        }

        timeCount = g_Time.CurrentTime();
        if (timeCount % flashSpeed == flashSpeed - 1) {
            if (isShowText && timeCount + 0.5f > g_Time.CurrentTime()) {
                m_TxtMessage->Color(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
                isShowText = false;
            }
            else if (!isShowText && timeCount + 0.5f <= g_Time.CurrentTime()) {
                m_TxtMessage->Color(glm::vec4(1.0f));
                isShowText = true;
            }
        }

        //Draw
        // Update managers
        //m_PhysicsManager.StepSimulation(g_Time.DeltaTime());
        m_ObjectManager.ProcessFrame();
        m_DrawManager.CallDraws();
        //g_IsPreview = false;
        CheckKeypress();
    }
    //End preview
    m_DrawManager.UnregisterGUIWidget(m_TxtMessage);
};

void Scene::Run() {
    // Initialize Time manager as close to game loop as possible
    // to avoid misrepresented delta time
    g_Time.Initialize();
    bool l_IsPreview = g_IsPreview;
    //Preprocess
    m_ObjectManager.ProcessFrame();
    g_IsPreview = l_IsPreview;

    float l_LastTime = g_Time.CurrentTime();
    float l_TimeMax = 4 + l_LastTime; //Second
    float l_CurTime = l_LastTime;
    int l_TimeCount = l_TimeMax - l_CurTime;

    std::cout << "IsPreview: " << g_IsPreview << std::endl;
    m_TxtMessage->Color(glm::vec4(1.0f));
    m_DrawManager.RegisterGUIWidget(m_TxtMessage);
    if (g_IsPreview && m_Running) {
        while (l_TimeCount > -1 && !glfwWindowShouldClose(g_Window)) {
            do {
                g_Time.Hold();
                glfwPollEvents();
            } while (g_Time.DeltaTime() < m_FrameRateLimit);
            // Update global systems
            g_Time.Update();
            g_Input.Update(g_Window);

            l_CurTime = g_Time.CurrentTime();
            if (l_CurTime != l_LastTime) {
                l_LastTime = l_CurTime;
                l_TimeCount = l_TimeMax - l_CurTime;
                if (l_TimeCount > 0) {
                    m_TxtMessage->Text(std::to_string(l_TimeCount));
                }
                else {
                    m_TxtMessage->Text("START!!!");
                }
            }

            m_DrawManager.CallDraws();
        }
        g_IsPreview = false;
    }

    //Time
    m_LastTimeChange = g_Time.CurrentTime();
    l_LastTime = g_Time.CurrentTime();
    l_TimeMax = m_LimitTime + l_LastTime; //Second
    l_CurTime = l_LastTime;
    l_TimeCount = l_TimeMax - l_CurTime;
    
    //Text
    m_TxtMessage->Position(glm::vec2(0.0f), TextRenderer::EAlign::BEGIN, TextRenderer::EAlign::BEGIN);
    m_TxtMessage->Color(glm::vec4(1.0f));

    // Game loop
    while (m_Running && !glfwWindowShouldClose(g_Window)) {
        // If frame rate is greater than limit then wait
        do {
            g_Time.Hold();
            glfwPollEvents();
        } while (g_Time.DeltaTime() < m_FrameRateLimit);
        
        // Update global systems
        g_Time.Update();
        g_Input.Update(g_Window);
        
        if (m_IsReset) {
            g_Score = 0;
            for (int i = 0; i < 10; i++) {
                g_BoxTypeCount[i] = 0;
            }

            if (m_IsChangeMap) {
                switch (m_MapCode)
                {
                case 1:
                    LoadMap("map_1.map");
                    break;
                case 2:
                    LoadMap("map_2.map");
                    break;
                case 3:
                    LoadMap("map_3.map");
                    break;
                default:
                    break;
                }

                //
                m_IsChangeMap = false;
            }

            m_MapOffset = 0;
            int curType;
            for (int i = 0; i < g_MapHeight; i++) {
                for (int j = 0; j < g_MapWidth; j++) {
                    if (g_BoxMap[g_MapHeight - i - 1][j] != NULL) {
                        delete g_BoxMap[g_MapHeight - i - 1][j];
                        g_BoxMap[g_MapHeight - i - 1][j] = NULL;
                    }
                    if (i < g_NumRowLoad) {
                        curType = m_BoxType[g_MapHeight_Max - g_NumRowLoad + i][j];
                        g_BoxMap[g_MapHeight - i - 1][j] = new Box(glm::vec3(g_OffsetFront,
                            g_OffsetTop - i * (g_BoxSize + g_BoxMargin),
                            g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)),
                            curType, false);

                        g_BoxMap[g_MapHeight - i - 1][j]->SetSize(glm::vec3(g_BoxSize));
                        g_BoxTypeCount[curType]++;
                    }
                }
            }

            m_LastTimeChange = g_Time.CurrentTime();
            l_LastTime = g_Time.CurrentTime();
            l_TimeMax = m_LimitTime + l_LastTime; //Second
            l_CurTime = l_LastTime;

            GenRedDot();

            g_BulletQueue.clear();
            //
            m_IsReset = false;
        }

        //Time
        l_CurTime = g_Time.CurrentTime();
        if (l_CurTime != l_LastTime) {
            l_LastTime = l_CurTime;
            l_TimeCount = l_TimeMax - l_CurTime;
        }

        //Fire
        if (g_IsFire) {
            g_RedDot->m_FlySpeed;
            Bullet* newBullet = new Bullet(g_RedDot);
            newBullet->m_FlySpeed *= 0.05f / g_RedDotSize;
            g_BulletQueue.push_back(newBullet);
            g_BulletQueue.back()->Fire();
            g_IsFire = false;
            GenRedDot();
            std::cout << "Bullet queue size: " << g_BulletQueue.size() << std::endl;
        }

        //--Bullet
        for (auto bullet : g_BulletQueue) {
            if (bullet->m_Status != -1) {
                if (-g_OffsetX.x + 50.0f < bullet->m_Position.x) {
                    bullet->m_Status = -1;
                    if (g_Score > 0) {
                        g_Score--;
                    }
                }
                else {
                    //Nảy nảy
                    if (g_OffsetX.x > bullet->m_Position.x && bullet->m_Direction.x < 0) {
                        bullet->m_Direction.x *= -1;
                        bullet->m_Status++;
                    }
                    if ((g_OffsetY.x > bullet->m_Position.y && bullet->m_Direction.y < 0) || (g_OffsetY.y < bullet->m_Position.y && bullet->m_Direction.y > 0)) {
                        bullet->m_Direction.y *= -1;
                        bullet->m_Status++;
                    }
                    if((g_OffsetZ.x > bullet->m_Position.z && bullet->m_Direction.z < 0) || (g_OffsetZ.y < bullet->m_Position.z && bullet->m_Direction.z > 0)){
                        bullet->m_Direction.z *= -1;
                        bullet->m_Status++;
                    }
                }
            }
            
            if (bullet->m_Status == -1) {
                auto to_erase = std::find(g_BulletQueue.begin(), g_BulletQueue.end(), bullet);
                if (to_erase != g_BulletQueue.end()) {
                    g_BulletQueue.erase(to_erase);
                }
            }
        }

        //--BoxMap
        int factor = 1;
        int killCount = 0;
        int killType = 0;
        bool isKill = false;
        for (int i = 0; i < g_MapHeight; i++) {
            for (int j = 0; j < g_MapWidth; j++) {
                if (g_BoxMap[i][j]) {
                    if (g_BoxMap[i][j]->m_Status == -1) {
                        factor = g_BoxMap[i][j]->m_Factor;
                        killType = g_BoxMap[i][j]->m_Type;
                        killCount = 0;
                        TryToKill(i, j, g_BoxMap[i][j]->m_Type, killCount);
                        g_Score += killCount * factor;
                        g_BoxTypeCount[killType] -= killCount;
                        isKill = true;
                        //l_TxtScore->Text("Score: " + to_string(g_Score));
                        break;
                    }
                }
            }
            if (isKill) {
                break;
            }
        }

        bool isLocalEnd = l_TimeCount <= 0;
        if (g_Time.CurrentTime() > m_LastTimeChange + m_TimeStep) {
            //Check end game
            if (!m_IsEndGame) {
                for (int j = 0; j < g_MapWidth; j++) {
                    if (g_BoxMap[0][j] != NULL) {
                        m_IsEndGame = true;
                        isLocalEnd = true;
                        break;
                    }
                }
            }

            if (!isLocalEnd) {
                m_MapOffset++;
                if (m_MapOffset + g_NumRowLoad > g_MapHeight_Max) {
                    //Chạy đến sáng
                    m_MapOffset = - g_NumRowLoad + 1;
                }
                for (int i = 0; i < g_MapHeight - 1; i++) {
                    for (int j = 0; j < g_MapWidth; j++) {
                        g_BoxMap[i][j] = g_BoxMap[i + 1][j];
                        if (g_BoxMap[i][j] != NULL) {
                            g_BoxMap[i][j]->Move(glm::vec3(0.0f, - (g_BoxSize + g_BoxMargin), 0.0f));
                        }
                    }
                }
                //Load hàng tiếp theo
                int curType;
                if (m_MapOffset + g_NumRowLoad <= g_MapHeight_Max) {
                    for (int j = 0; j < g_MapWidth; j++) {
                        curType = m_BoxType[g_MapHeight_Max - m_MapOffset - g_NumRowLoad][j];
                        g_BoxMap[g_MapHeight - 1][j] = new Box(glm::vec3(g_OffsetFront, g_OffsetTop, g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)),
                            curType, false);
                        g_BoxMap[g_MapHeight - 1][j]->SetSize(glm::vec3(g_BoxSize));
                        g_BoxTypeCount[curType]++;
                    }
                }

                m_LastTimeChange = g_Time.CurrentTime();
                std::cout << "-- Time: " << m_LastTimeChange << ": Box map change (offset: " << m_MapOffset << ")" << std::endl;
            }
        }

        if (isLocalEnd) {
            std::cout << "--------------------------------------------" << std::endl << "END GAME!" << std::endl;
            wstring tempStr = L"Bạn đã thua với số điểm " + std::to_wstring(g_Score) + L"!!!\n => Nhấn OK để làm lại! \n => Nhấn Cancel để không làm lại!";
            LPCWSTR str(tempStr.c_str());
            int endSelected = MessageBox(NULL, str, TEXT("Trò chơi đoán rằng:"), MB_ICONINFORMATION | MB_OKCANCEL);
            if (endSelected == IDOK) {
                ResetGame();
            }
            else {
                //Exit();
            }
        }

        // Update managers
        //m_PhysicsManager.StepSimulation(g_Time.DeltaTime());
        m_TxtMessage->Text("Score: " + std::to_string(g_Score) + "\nTime: " + ToTime(l_TimeCount));

        m_ObjectManager.ProcessFrame();
        m_DrawManager.CallDraws();
        CheckKeypress();
    }
}

void Scene::Destroy() {
    m_ObjectManager.DestroyObjects();
    //m_PhysicsManager.ExitPhysics();
}

void Scene::ResetGame() {
    m_IsEndGame = false;
    m_IsReset = true;
}

void Scene::Exit() {
    m_Running = false;
}

void Scene::FrameRateLimit(unsigned int frame_rate) {
    m_FrameRateLimit = frame_rate != 0 ? 1.0f / (float)frame_rate : 0.0f;
}


Object* Scene::CreateObject(const std::string& name) {
    return m_ObjectManager.CreateObject(name);
}

void Scene::DestroyObject(Object::ID_t id) {
    m_ObjectManager.DestroyObject(id);
}


void Scene::RegisterDrawCall(IDrawable* drawable, EShaderType shader) {
    m_DrawManager.RegisterDrawCall(drawable, shader);
}

void Scene::UnregisterDrawCall(IDrawable* drawable, EShaderType shader) {
    m_DrawManager.UnregisterDrawCall(drawable, shader);
}

void Scene::RegisterShaderProperty(const IShaderProperty* property, EShaderType shader) {
    m_DrawManager.RegisterShaderProperty(property, shader);
}

void Scene::UnregisterShaderProperty(const IShaderProperty* property, EShaderType shader) {
    m_DrawManager.UnregisterShaderProperty(property, shader);
}

void Scene::RegisterGUIWidget(IGUIWidget* widget) {
    m_DrawManager.RegisterGUIWidget(widget);
}

void Scene::UnregisterGUIWidget(IGUIWidget* widget) {
    m_DrawManager.UnregisterGUIWidget(widget);
}

void Scene::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    m_DrawManager.DrawLine(start, end, color);
}

void Scene::DrawPlane(glm::mat4 model, glm::vec3 color) {
    m_DrawManager.DrawPlane(model, color);
}

void Scene::DrawCuboid(glm::mat4 model, glm::vec3 color) {
    m_DrawManager.DrawCuboid(model, color);
}

void Scene::DrawCuboid(glm::mat4 model, glm::vec3 color, std::string texturePath) {
    m_DrawManager.DrawCuboid(model, color, texturePath);
}

void Scene::DrawSphere(glm::mat4 model, glm::vec3 color) {
    //TODO
}

void Scene::RegisterCamera(Camera* camera) {
    m_DrawManager.RegisterCamera(camera);
}

Camera* Scene::MainCamera() const {
    return m_DrawManager.MainCamera();
}

void Scene::Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front) {
    m_DrawManager.Skybox(right, left, top, bottom, back, front);
}

void Scene::Background(const glm::vec3& background) {
    m_DrawManager.Background(background);
}


//void Scene::AddCollisionObject(btCollisionObject* collision_object, int collision_filter_group, int collision_filter_mask) {
//    m_PhysicsManager.AddCollisionObject(collision_object, collision_filter_group, collision_filter_mask);
//}

//void Scene::RemoveCollisionObject(btCollisionObject* collision_object) {
//    m_PhysicsManager.RemoveCollisionObject(collision_object);
//}
//
//void Scene::AddRigidBody(btRigidBody* rigid_body, int group, int mask) {
//    m_PhysicsManager.AddRigidBody(rigid_body, group, mask);
//}
//
//void Scene::RemoveRigidBody(btRigidBody* rigid_body) {
//    m_PhysicsManager.RemoveRigidBody(rigid_body);
//}
//
//void Scene::AddConstraint(btTypedConstraint* constraint, bool disable_collisions_between_linked_bodies) {
//    m_PhysicsManager.AddConstraint(constraint, disable_collisions_between_linked_bodies);
//}
//
//void Scene::RemoveConstraint(btTypedConstraint* constraint) {
//    m_PhysicsManager.RemoveConstraint(constraint);
//}
//
//void Scene::AddVehicle(btRaycastVehicle* vehicle) {
//    m_PhysicsManager.AddVehicle(vehicle);
//}
//
//void Scene::RemoveVehicle(btRaycastVehicle* vehicle) {
//    m_PhysicsManager.RemoveVehicle(vehicle);
//}
//
//void Scene::Raycast(const btVector3& from, const btVector3& to, btCollisionWorld::RayResultCallback& result) {
//    m_PhysicsManager.Raycast(from, to, result);
//}
//
//void Scene::Gravity(const btVector3& gravity) {
//    m_PhysicsManager.Gravity(gravity);
//}
//
//btVector3 Scene::Gravity() const {
//    return m_PhysicsManager.Gravity();
//}

//btDynamicsWorld* Scene::DynamicsWorld() {
//    return m_PhysicsManager.DynamicsWorld();
//}
//

//void Scene::ListenerPosition(float x, float y, float z) {
//    m_AudioManager.ListenerPosition(x, y, z);
//}
//
//void Scene::ListenerVelocity(float x, float y, float z) {
//    m_AudioManager.ListenerVelocity(x, y, z);
//}
//
//void Scene::ListenerGain(float gain) {
//    m_AudioManager.ListenerGain(gain);
//}
//
//void Scene::ListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z) {
//    m_AudioManager.ListenerOrientation(at_x, at_y, at_z, up_x, up_y, up_z);
//}


RawTexture& Scene::LoadTexture(const std::string& path) {
    return m_ResourceManager.LoadTexture(path);
}

RawModel& Scene::LoadModel(const std::string& path) {
    return m_ResourceManager.LoadModel(path);
}

//Sound& Scene::LoadSound(const std::string& path) {
//    return m_ResourceManager.LoadSound(path);
//}

