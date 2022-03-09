#include "Scene.h"

//Scene::Scene() 
//    : m_ObjectManager(*this)
//    , m_PhysicsManager(new PhysicsRenderer(m_DrawManager)) {
//}

//Thuat toan
void TryToKill(int posY, int posX, int typeToKill) {
    if (g_BoxMap[posY][posX] != NULL && g_BoxMap[posY][posX]->m_Type == typeToKill) {
        g_Score++;
        delete g_BoxMap[posY][posX];
        g_BoxMap[posY][posX] = NULL;

        //Loang
        if (posY > 0 && g_BoxMap[posY - 1][posX] != NULL) {
            TryToKill(posY - 1, posX, typeToKill);
        }
        if (posY < g_MapHeight - 1 && g_BoxMap[posY + 1][posX] != NULL) {
            TryToKill(posY + 1, posX, typeToKill);
        }
        if (posX > 0 && g_BoxMap[posY][posX - 1] != NULL) {
            TryToKill(posY, posX - 1, typeToKill);
        }
        if (posX < g_MapWidth - 1 && g_BoxMap[posY][posX + 1] != NULL) {
            TryToKill(posY, posX + 1, typeToKill);
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

void Scene::ResetGame() {
    //
    m_IsEndGame = false;
    g_Score = 0;
    m_MapOffset = 0;
    for (int i = 0; i < g_MapHeight; i++) {
        for (int j = 0; j < g_MapWidth; j++) {
            if (g_BoxMap[g_MapHeight - i - 1][j] != NULL) {
                delete g_BoxMap[g_MapHeight - i - 1][j];
                g_BoxMap[g_MapHeight - i - 1][j] = NULL;
            }
            if (i < g_NumRowLoad) {
                g_BoxMap[g_MapHeight - i - 1][j] = new Box(glm::vec3(g_OffsetFront,
                    g_OffsetTop - i * (g_BoxSize + g_BoxMargin),
                    g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)),
                    m_BoxType[g_MapHeight_Max - g_NumRowLoad + i][j], false);

                g_BoxMap[g_MapHeight - i - 1][j]->SetSize(glm::vec3(g_BoxSize));
            }
        }
    }

    m_LastTimeChange = g_Time.CurrentTime();
}

void Scene::Preview() {

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

    while (g_IsPreview && !glfwWindowShouldClose(g_Window)) {
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
            else if (!isShowText && timeCount + 0.5f <= g_Time.CurrentTime()) {
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

void Scene::Run() {
    m_Running = true;

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
    m_DrawManager.RegisterGUIWidget(m_TxtMessage);
    if (g_IsPreview) {
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
    l_TimeMax = 300 + l_LastTime; //Second
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
        
        //Time
        l_CurTime = g_Time.CurrentTime();
        if (l_CurTime != l_LastTime) {
            l_LastTime = l_CurTime;
            l_TimeCount = l_TimeMax - l_CurTime;
        }

        //Fire
        if (g_IsFire) {
            Bullet* newBullet = new Bullet(g_RedDot);
            g_BulletQueue.push_back(newBullet);
            g_BulletQueue.back()->Fire();
            g_IsFire = false;
            int newBulletType = rand() % 9 + 1;
            g_RedDot->m_Type = newBulletType;
            g_RedDot->m_Color = Box::GenColor(newBulletType);
            std::cout << "Bullet size: " << g_BulletQueue.size() << std::endl;
        }

        //--Bullet
        for (auto bullet : g_BulletQueue) {
            if ((g_OffsetX.x > bullet->m_Position.x || -g_OffsetX.x < bullet->m_Position.x)
                || (-g_OffsetY.y > bullet->m_Position.y || g_OffsetY.y < bullet->m_Position.y)
                || (g_OffsetZ.x > bullet->m_Position.z || g_OffsetZ.y < bullet->m_Position.z)) {
                bullet->m_Status = -1;
            }
            if (bullet->m_Status == -1) {
                auto to_erase = std::find(g_BulletQueue.begin(), g_BulletQueue.end(), bullet);
                if (to_erase != g_BulletQueue.end()) {
                    g_BulletQueue.erase(to_erase);
                }
            }
        }

        //--BoxMap
        for (int i = 0; i < g_MapHeight; i++) {
            bool isKill = false;
            for (int j = 0; j < g_MapWidth; j++) {
                if (g_BoxMap[i][j]) {
                    if (g_BoxMap[i][j]->m_Status == -1) {
                        TryToKill(i, j, g_BoxMap[i][j]->m_Type);
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

        if (g_Time.CurrentTime() > m_LastTimeChange + m_TimeStep) {
            //Check end game
            bool isLocalEnd = false;
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
                if (m_MapOffset + g_NumRowLoad <= g_MapHeight_Max) {
                    for (int j = 0; j < g_MapWidth; j++) {
                        g_BoxMap[g_MapHeight - 1][j] = new Box(glm::vec3(g_OffsetFront, g_OffsetTop, g_OffsetLeft + j * (g_BoxSize + g_BoxMargin)),
                            m_BoxType[g_MapHeight_Max - m_MapOffset - g_NumRowLoad][j], false);
                        g_BoxMap[g_MapHeight - 1][j]->SetSize(glm::vec3(g_BoxSize));
                    }
                }

                m_LastTimeChange = g_Time.CurrentTime();
                std::cout << "-- Time: " << m_LastTimeChange << ": Box map change (offset: " << m_MapOffset << ")" << std::endl;
            }
            else {
                std::cout << "--------------------------------------------" << std::endl << "END GAME!" << std::endl;
                wstring tempStr = L"Bạn đã thua với số điểm " + std::to_wstring(g_Score) + L"!!!\n => Nhấn OK để làm lại! \n => Nhấn Cancel để không làm lại!";
                LPCWSTR str(tempStr.c_str());
                int endSelected = MessageBox(NULL, str, TEXT("Trò chơi đoán rằng:"), MB_ICONINFORMATION | MB_OKCANCEL);
                if (endSelected == IDOK) {
                    ResetGame();
                }
            }
        }

        // Update managers
        //m_PhysicsManager.StepSimulation(g_Time.DeltaTime());
        m_TxtMessage->Text("Score: " + std::to_string(g_Score) + "\nTime: " + ToTime(l_TimeCount));

        m_ObjectManager.ProcessFrame();
        m_DrawManager.CallDraws();
    }
}

void Scene::Destroy() {
    m_ObjectManager.DestroyObjects();
    //m_PhysicsManager.ExitPhysics();
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

