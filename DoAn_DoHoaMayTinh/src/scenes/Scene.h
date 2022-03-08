#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
using namespace std;

//#include "../physics/PhysicsManager.h"
#include "../cbs/ObjectManager.h"
//#include "../rendering/DrawManager.h"
#include "../resources/ResourcesManager.h"
//#include "../audio/AudioManager.h"

#include "../utilities/Time.h"
#include "../utilities/Input.h"
#include "../utilities/Window.h"

//Game object
#include "../gameobject/Box.h"

//Text
#include "../cbs/components/TextRenderer.h"

class Scene {
public:
    Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;
    virtual ~Scene() = default;

    // 
    void Initialize();
    virtual void PreRun() {}
    virtual void CreateScene() = 0;
    void Run();
    virtual void PostRun() {}
    void Destroy();

    // Scene functions
    void Exit();
    void FrameRateLimit(unsigned int frame_rate);
    float FrameRateLimit() const { return m_FrameRateLimit; }
    float FrameRate() const { return 1.0f / g_Time.DeltaTime(); }
    void ResetGame();

    // ObjectManager functions
    Object* CreateObject(const std::string& name);
    void DestroyObject(Object::ID_t id);

    // DrawManager functions
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
    void PushDrawable(IDrawable* obj) {
        m_DrawManager.PushDrawable(obj);
    }

    void RegisterCamera(Camera* camera);
    Camera* MainCamera() const;
    void Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front);
    void Background(const glm::vec3& background);

    // PhysicsManager functions
    /*void AddCollisionObject(btCollisionObject* collision_object, int collision_filter_group = 1, int collision_filter_mask = -1);
    void RemoveCollisionObject(btCollisionObject* collision_object);
    void AddRigidBody(btRigidBody* rigid_body, int group = 1, int mask = -1);
    void RemoveRigidBody(btRigidBody* rigid_body);
    void AddConstraint(btTypedConstraint* constraint, bool disable_collisions_between_linked_bodies = false);
    void RemoveConstraint(btTypedConstraint* constraint);
    void AddVehicle(btRaycastVehicle* vehicle);
    void RemoveVehicle(btRaycastVehicle* vehicle);
    void Raycast(const btVector3& from, const btVector3& to, btCollisionWorld::RayResultCallback& result);
    void Gravity(const btVector3& gravity);
    btVector3 Gravity() const;
    btDynamicsWorld* DynamicsWorld();*/

    // AudioManager functions
    /*void ListenerPosition(float x, float y, float z);
    void ListenerVelocity(float x, float y, float z);
    void ListenerGain(float gain);
    void ListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);*/

    // ResourceManager functions
    RawTexture& LoadTexture(const std::string& path);
    RawModel& LoadModel(const std::string& path);
    //Sound& LoadSound(const std::string& path);

protected:
    bool m_IsEndGame = false;
    int m_BoxType[230][20];
    //Draw object
    int m_MapOffset;
    float m_TimeStep; //Thời gian chờ boxmap đi xuống
    float m_LastTimeChange;
    TextRenderer* m_TxtMessage;
    //
    bool m_Running{ false };
    float m_FrameRateLimit{ 0.0f };
//private:
    ObjectManager m_ObjectManager;
    DrawManager m_DrawManager;
    /*PhysicsManager m_PhysicsManager;*/
    //AudioManager m_AudioManager;
    ResourcesManager m_ResourceManager;
};

extern bool g_IsFire;
extern int g_Score;
extern float g_OffsetLeft;
extern float g_OffsetTop;
extern float g_OffsetBottom;
extern float g_OffsetFront;
#endif
