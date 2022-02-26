#include "Camera.h"

// Undef Visual Studio macros
#undef far
#undef near

Camera::Camera() {
    //
}

Camera::Camera(glm::mat4 projection)
    : m_ViewMatrix(0.0f)
    , m_Projection(projection)
    , m_Transform(nullptr) {
    
}

Camera::Camera(float fovy, float aspect, float near, float far)
    : m_ViewMatrix(0.0f)
    , m_Projection(Perspective(fovy, aspect, near, far))
    , m_Transform(nullptr) {
}

Camera::Camera(float left, float right, float bottom, float top, float near, float far)
    : m_ViewMatrix(0.0f)
    , m_Projection(Orthographic(left, right, bottom, top, near, far))
    , m_Transform(nullptr) {
    
}

void Camera::Initialize() {
    Object().Scene().RegisterCamera(this);
    m_Transform = &Object().Root();
}

glm::mat4 Camera::ViewMatrix() const {
    glm::vec3 preObjectBottom = m_Transform->Position();
    glm::vec3 cameraPosition = preObjectBottom + g_CameraOffset;
    return glm::lookAt(cameraPosition,
                       cameraPosition + m_Transform->Front(),
                       m_Transform->Up());
}

glm::mat4 Camera::Perspective(float fovy, float aspect, float near, float far) {
    return glm::perspective(fovy, aspect, near, far);
}

glm::mat4 Camera::Orthographic(float left, float right, float bottom, float top, float near, float far) {
    return glm::ortho(left, right, bottom, top, near, far);
}

