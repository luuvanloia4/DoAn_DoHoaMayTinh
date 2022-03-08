#ifndef Transform_h
#define Transform_h

#include "Component.h"
#include "../connections/PropertyOut.h"
#include "../connections/PropertyIn.h"

#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../../utilities/Time.h"
#pragma warning(pop)


extern glm::vec2 g_OffsetX;
extern glm::vec2 g_OffsetY;
extern glm::vec2 g_OffsetZ;
extern float g_Gravity;

class Transform : public Component {
public:
    void Initialize() override;

    void Identity();

    glm::mat4 Model() const;
    void Model(const glm::mat4 model);
    void Model(const float* model);

    glm::vec3 Position() const;
    void Position(const glm::vec3& position);
    void Move(const glm::vec3& vector);
    void MoveWithoutRotation(const glm::vec3& vector);

    glm::quat Rotation() const;
    void Rotation(const glm::quat& rotation);
    void Rotate(const glm::quat& rotation);
    void RotateRelative(const glm::quat& rotation);

    glm::vec3 Scale() const;
    void Scale(const glm::vec3& scale);

    //Force
    void AddForce(int forceType, float strength, glm::vec3 direction);
    void UpdateForceStatus();
    int GetForceStatus();

    //Collision
    int CheckCollision(glm::vec3 pos, glm::vec3 size);

    glm::vec3 Front() { return m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f); }
    glm::vec3 Up() { return m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f); }
    glm::vec3 Right() { return m_Rotation * glm::vec3(0.0f, 0.0f, 1.0f); }

    PropertyOut<Transform*> This{ this, this };
    PropertyIn<Transform*> Parent{ this };

private:
    void UpdateModel();

    glm::mat4 m_Model{ 1.0f };
    glm::vec3 m_Position{ 0.0f };
    glm::quat m_Rotation{ glm::vec3(0.0f) };
    glm::vec3 m_Scale{ 1.0f };

    //Force
    int m_ForceType = 0;
    int m_ForceStatus = 0;
    float m_LastTimeInTop;
    float m_ForceStrength = 0.0f;
    glm::vec3 m_ForceDirection = glm::vec3(0.0f);

    //Collision
    int m_CollisionType = 0; // 0: none, 1: top, 2: bottom, 3: left, 4: right, 5: front, 6 back
};

extern glm::quat g_CameraRotation;
#endif
