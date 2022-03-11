#include "FirstPersonController.h"
#include "Transform.h"

FirstPersonController::FirstPersonController(float movement_speed_fast, float movement_speed_slow, float mouse_sensivity)
    : m_CurrentMovementSpeed(0.0f)
    , m_MovementSpeedFast(movement_speed_fast)
    , m_MovementSpeedSlow(movement_speed_slow)
    , m_MouseSensitivity(mouse_sensivity)
    , m_VerticalRotation(0.0f)
    , m_LastMousePos(0.0f) {}

void FirstPersonController::Initialize() {
    // TODO store original rotation from Transform component
    m_LastMousePos = g_Input.MousePosition();
    m_IsFree = false;

    RegisterUpdateCall();
}

void FirstPersonController::Update() {
    // Mouse
    if (!g_Input.KeyHold(GLFW_MOUSE_BUTTON_2)) {
        float rot_ver = glm::radians(g_Input.MouseOffset().y * m_MouseSensitivity);
        float rot_hor = glm::radians(-g_Input.MouseOffset().x * m_MouseSensitivity);
        m_VerticalRotation += rot_ver;

        // Prevent object flip
        if (m_VerticalRotation > DEGREES_60) {
            m_VerticalRotation = DEGREES_60;
            rot_ver = 0.0f;
        }
        else if (m_VerticalRotation < -DEGREES_60) {
            m_VerticalRotation = -DEGREES_60;
            rot_ver = 0.0f;
        }

        // Update vectors
        TransformIn.Value()->Rotate(glm::vec3(0.0f, rot_hor, 0.0f));
        //TransformIn.Value()->RotateRelative(glm::vec3(0.0f, 0.0f, rot_ver));
    }

    //Fire
    if (g_Input.KeyPressed(GLFW_MOUSE_BUTTON_LEFT) || g_Input.KeyHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (g_RedDotSize >= 0.01f) {
            g_RedDotSize -= g_Time.DeltaTime() * 0.03f;
        }
    }

    if (g_IsPreview) {
        //Chế độ preview bỏ qua mọi định luật vật lý
        if (g_Input.KeyReleased(GLFW_MOUSE_BUTTON_LEFT)) {
            g_RedDotSize = 0.05f;
        }

        // Keyboard
        m_CurrentMovementSpeed = m_MovementSpeedFast;
        glm::vec3 cameraFont = g_CameraRotation * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 cameraRight = g_CameraRotation * glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 movement(0.0f);
        if (g_Input.KeyHold(GLFW_KEY_W)) {
            movement += m_CurrentMovementSpeed * g_Time.DeltaTime() * cameraFont;
        }
        if (g_Input.KeyHold(GLFW_KEY_S)) {
            movement += -m_CurrentMovementSpeed * g_Time.DeltaTime() * cameraFont;
        }
        if (g_Input.KeyHold(GLFW_KEY_A)) {
            movement += -m_CurrentMovementSpeed * g_Time.DeltaTime() * cameraRight;
        }
        if (g_Input.KeyHold(GLFW_KEY_D)) {
            movement += m_CurrentMovementSpeed * g_Time.DeltaTime() * cameraRight;
        }

        //std::cout << "Movement: " << movement.x << " | " << movement.y << " | " << movement.z << std::endl;
        TransformIn.Value()->MoveWithoutRotation(movement);

        //Thoát khỏi chế độ preview
        if (g_Input.KeyPressed(GLFW_KEY_SPACE)) {
            g_IsPreview = false;
        }
    }
    else {
        if (g_Input.KeyReleased(GLFW_MOUSE_BUTTON_LEFT)) {
            std::cout << "RELEASED LEFT BUTTON" << std::endl;
            g_IsFire = true;
            //TransformIn.Value()->AddForce(2, 0.2f, glm::vec3(1.0f, 1.0f, 1.0f)); //Đẩy lui khi bắn //Nhưng thôi k làm
            g_RedDotSize = 0.05f;
            //Fire in Scene->Run()
        }

        // Keyboard
        if (g_Input.KeyHold(GLFW_KEY_LEFT_SHIFT) || g_Input.KeyHold(GLFW_KEY_RIGHT_SHIFT)) {
            m_CurrentMovementSpeed = m_MovementSpeedFast;
        }
        else {
            m_CurrentMovementSpeed = m_MovementSpeedSlow;
        }

        glm::vec3 movement(0.0f);
        if (g_Input.KeyHold(GLFW_KEY_W)) {
            movement.x = -m_CurrentMovementSpeed * g_Time.DeltaTime();
        }
        if (g_Input.KeyHold(GLFW_KEY_S)) {
            movement.x = m_CurrentMovementSpeed * g_Time.DeltaTime();
        }
        if (g_Input.KeyHold(GLFW_KEY_A)) {
            movement.z = m_CurrentMovementSpeed * g_Time.DeltaTime();
        }
        if (g_Input.KeyHold(GLFW_KEY_D)) {
            movement.z = -m_CurrentMovementSpeed * g_Time.DeltaTime();
        }
        TransformIn.Value()->Move(movement);

        if (g_Input.KeyPressed(GLFW_KEY_SPACE)) {
            int forceStatus = TransformIn.Value()->GetForceStatus();
            if (forceStatus == 0) {
                TransformIn.Value()->AddForce(1, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else if (forceStatus == 1) {
                // Đắp bồ giăm
                TransformIn.Value()->AddForce(1, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else {
                //
            }
        }

        TransformIn.Value()->UpdateForceStatus();
    }
}
