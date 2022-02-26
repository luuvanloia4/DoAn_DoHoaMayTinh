#include "MainScene.h"

#include "../cbs/components/MeshRenderer.h"
#include "../cbs/components/RigidBody.h"
#include "../cbs/components/Camera.h"
#include "../cbs/components/FirstPersonController.h"
#include "../cbs/components/ThirdPersonController.h"
#include "../cbs/components/DirectionalLight.h"
#include "../cbs/components/PointLight.h"
#include "../cbs/components/Dummy.h"
#include "../cbs/components/Vehicle.h"
#include "../cbs/components/Controller.h"
#include "../cbs/components/GhostObject.h"
#include "../cbs/components/Cube.h"
#include "../cbs/components/TextRenderer.h"
#include "../cbs/components/AudioSource.h"
#include "../cbs/components/World/World.h"

void MainScene::CreateScene() {
    FrameRateLimit(60);
    Skybox("resources/skyboxes/mars/right.png",
           "resources/skyboxes/mars/left.png",
           "resources/skyboxes/mars/top.png",
           "resources/skyboxes/mars/bottom.png",
           "resources/skyboxes/mars/back.png",
           "resources/skyboxes/mars/front.png");

    auto light = CreateObject("Light"); {
        light->CreateComponent<DirectionalLight>(glm::vec3(-1.0f, -0.5f, -0.5f),
                                                 glm::vec3(0.05f),
                                                 glm::vec3(0.7f, 0.68f, 0.68f),
                                                 glm::vec3(0.8f, 0.78f, 0.78f));
    }

    auto rover = CreateObject("Rover"); {
        rover->Root().Move(glm::vec3(10.0f, 0.5f, 10.0f));

        // Rigid body
        auto rigid_body = rover->CreateComponent<RigidBody>(250, new btBoxShape(btVector3(0.25f, 0.12f, 0.25f)));
        rover->Connect(rover->Root().This, rigid_body->TransformIn);


        // Mesh
        auto mesh_transform = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, mesh_transform->Parent);
        mesh_transform->Scale(glm::vec3(0.05f));

        auto chasis_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_no_wheels.obj"), EShaderType::Phong);
        rover->Connect(mesh_transform->This, chasis_mesh->TransformIn);


        // Wheels
        auto front_left = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, front_left->Parent);
        front_left->Move(glm::vec3(-0.4f, -0.3f, -0.55f));
        front_left->Scale(glm::vec3(0.05f));

        auto wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(front_left->This, wheel_mesh->TransformIn);


        auto front_right = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, front_right->Parent);
        front_right->Move(glm::vec3(0.4f, -0.3f, -0.55f));
        front_left->Rotate(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        front_right->Scale(glm::vec3(0.05f));

        wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(front_right->This, wheel_mesh->TransformIn);


        auto middle_left = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, middle_left->Parent);
        middle_left->Move(glm::vec3(-0.4f, -0.3f, 0.0f));
        middle_left->Rotate(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        middle_left->Scale(glm::vec3(0.05f));

        wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(middle_left->This, wheel_mesh->TransformIn);


        auto middle_right = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, middle_right->Parent);
        middle_right->Move(glm::vec3(0.4f, -0.3f, 0.0f));
        middle_right->Scale(glm::vec3(0.05f));

        wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(middle_right->This, wheel_mesh->TransformIn);


        auto back_left = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, back_left->Parent);
        back_left->Move(glm::vec3(-0.4f, -0.3f, 0.4f));
        back_left->Rotate(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        back_left->Scale(glm::vec3(0.05f));

        wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(back_left->This, wheel_mesh->TransformIn);


        auto back_right = rover->CreateComponent<Transform>();
        rover->Connect(rover->Root().This, back_right->Parent);
        back_right->Move(glm::vec3(0.4f, -0.3f, 0.4f));
        back_right->Scale(glm::vec3(0.05f));

        wheel_mesh = rover->CreateComponent<MeshRenderer>(LoadModel("resources/models/opportunity/oppy_wheel.obj"), EShaderType::Phong);
        rover->Connect(back_right->This, wheel_mesh->TransformIn);


        // Vehicle
        auto vehicle = rover->CreateComponent<Vehicle>(0.0f, 0.3f, 0.1f, 0.1f);
        rover->Connect(rigid_body->This, vehicle->Chassis);
        rover->Connect(front_left->This, vehicle->FrontLeftWheel);
        rover->Connect(front_right->This, vehicle->FrontRightWheel);
        rover->Connect(middle_left->This, vehicle->MiddleLeftWheel);
        rover->Connect(middle_right->This, vehicle->MiddleRightWheel);
        rover->Connect(back_left->This, vehicle->BackLeftWheel);
        rover->Connect(back_right->This, vehicle->BackRightWheel);


        // Controller
        auto controller = rover->CreateComponent<Controller>(50.0f, 20.0f, glm::radians(1.0f), glm::radians(30.0f));
        rover->Connect(vehicle->This, controller->ControlledVehicle);
    }

    auto camera = CreateObject("Camera"); {
        camera->Root().Move(glm::vec3(0.0f, 1.0f, 5.0f));
        camera->CreateComponent<Camera>(glm::perspective(glm::radians(45.0f), static_cast<float>(g_Window.Width()) / static_cast<float>(g_Window.Height()), 0.1f, 500.0f));

        //auto fpc = camera->CreateComponent<FirstPersonController>();
        //camera->Connect(camera->Root().This, fpc->TransformIn);

        auto tpc = camera->CreateComponent<ThirdPersonController>(rover, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
        camera->Connect(camera->Root().This, tpc->TransformIn);
    }

    auto world = CreateObject("World"); {
        auto controller = world->CreateComponent<World>(rover, 9);
        controller->Background = &LoadSound("resources/audio/ambient.wav");

        auto background_source = world->CreateComponent<AudioSource>();
        world->Connect(controller->Background, background_source->SoundIn);
        background_source->Loop(true);
        background_source->Play();
    }
}
