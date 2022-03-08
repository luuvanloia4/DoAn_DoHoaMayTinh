#include "Transform.h"

#include "../Object.h"

void Transform::Initialize() {
    UpdateModel();
}

void Transform::Identity() {
    m_Position = glm::vec3(0.0f);
    m_Rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
    m_Scale = glm::vec3(1.0f);

    m_Model = glm::mat4(1.0f);
}

glm::mat4 Transform::Model() const {
    if (Parent.Connected()) {
        glm::mat4 model = glm::translate(Parent.Value()->Model(), m_Position) * glm::toMat4(m_Rotation);
        model = glm::scale(model, m_Scale);
        return model;
    } else {
        return m_Model;
    }
}

void Transform::Model(const glm::mat4 model) {
    m_Model = model;
}

void Transform::Model(const float* model) {
    m_Model = glm::make_mat4(model);
}

glm::vec3 Transform::Position() const {
    if (Parent.Connected()) {
        return glm::vec3(Model()[3]);
    } else {
        return m_Position;
    }
}

void Transform::Position(const glm::vec3& position) {
    m_Position = position;

    UpdateModel();
}

void Transform::Move(const glm::vec3& vector) {
    m_Position = m_Position + m_Rotation * vector;
    //X limit
    m_Position.x = (m_Position.x < g_OffsetX.x) ? g_OffsetX.x : m_Position.x;
    m_Position.x = (m_Position.x > g_OffsetX.y) ? g_OffsetX.y : m_Position.x;
    //Y limit
    m_Position.y = (m_Position.y < g_OffsetY.x) ? g_OffsetY.x : m_Position.y;
    m_Position.y = (m_Position.y > g_OffsetY.y) ? g_OffsetY.y : m_Position.y;
    //Z limit
    m_Position.z = (m_Position.z < g_OffsetZ.x) ? g_OffsetZ.x : m_Position.z;
    m_Position.z = (m_Position.z > g_OffsetZ.y) ? g_OffsetZ.y : m_Position.z;

    //std::cout << m_Position.x << " () " << m_Position.y << std::endl;

    UpdateModel();
}

void Transform::MoveWithoutRotation(const glm::vec3& vector) {
    //Chế độ preview bỏ qua mọi định luật vật lý
    m_Position = m_Position + vector;

    UpdateModel();
}

glm::quat Transform::Rotation() const {
    if (Parent.Connected()) {
        glm::quat rotation;
        glm::vec3 tmp1, tmp2, tmp3;
        glm::vec4 tmp4;
        glm::decompose(Model(), tmp1, rotation, tmp2, tmp3, tmp4);

        return rotation;
    } else {
        return m_Rotation;
    }
}

void Transform::Rotation(const glm::quat &rotation) {
    m_Rotation = rotation;

    UpdateModel();
}

void Transform::Rotate(const glm::quat& rotation) {
    m_Rotation = rotation * m_Rotation;

    UpdateModel();
}

void Transform::RotateRelative(const glm::quat& rotation) {
    m_Rotation = m_Rotation * rotation;

    UpdateModel();
}

glm::vec3 Transform::Scale() const {
    if (Parent.Connected()) {
        return m_Scale * Parent.Value()->Scale();
    } else {
        return m_Scale;
    }
}

void Transform::Scale(const glm::vec3& scale) {
    m_Scale = scale;
    
    UpdateModel();
}

void Transform::UpdateModel() {
    if (Parent.Connected()) {
        m_Model = Parent.Value()->Model();
    } else {
        m_Model = glm::mat4(1.0f);
    }

    m_Model = glm::translate(m_Model, m_Position);
    //m_Model = m_Model * glm::toMat4(m_Rotation);
    m_Model = glm::scale(m_Model, m_Scale);
}

void Transform::AddForce(int forceType, float strength, glm::vec3 direction) {
    //Type:
    // 0: normal
    // 1: jump
    // 2: comming soon...
    //
    m_ForceType = forceType;
    m_ForceStrength = (strength < 0.0f)? 0.0f: strength;
    m_ForceDirection = direction;
    if (m_ForceStatus == 0) {
        m_ForceStatus = 1;
    }
    else if (m_ForceStatus == 1) {
        m_ForceStatus = 2;
    }
}

void Transform::UpdateForceStatus() {
    if (m_ForceType == 0) {
        //Bình thường mà, không cần xử lý làm gì
    }
    else {
        //Nhay
        if (m_ForceType == 1) {
            m_Position += m_ForceStrength * m_ForceDirection;
            UpdateModel();

            if (m_Position.y > g_OffsetY.x) {
                m_ForceStrength += (-g_Gravity * g_Time.DeltaTime());
            }
            else {
                m_Position.y = g_OffsetY.x;
                m_ForceStrength = 0.0f;
                m_ForceType = 0; //End add force script
                m_ForceStatus = 0;
            }
        }

        if (m_ForceStrength != 0.0f) {
            //
        }
        else {
            //
        }
    }
}

int Transform::GetForceStatus() {
    return m_ForceStatus;
}

//Collision
//int Transform::CheckCollision(glm::vec3 pos, glm::vec3 size) {
//    glm::vec3 sumSize = m_Scale + size;
//    float offsetX = abs(m_Position.x - pos.x);
//    float offsetY = abs(m_Position.y - pos.y);
//    float offsetZ = abs(m_Position.z - pos.z);
//
//    if (offsetX <= sumSize.x && offsetY <= sumSize.y && offsetZ <= sumSize.z) {
//        if(offsetX )
//    }
//}
