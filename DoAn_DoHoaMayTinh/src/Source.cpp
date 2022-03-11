
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

//GLAD
//#include<glad/glad.h>

////GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>
//GLFW
//#include <GL/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
//

#include "utilities/Time.h"
#include "utilities/Input.h"
#include "utilities/Window.h"

#include "scenes/MyScene.h"

//#include "ShaderProgram.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0

const GLint WIDTH = 1366, HEIGHT = 768;

// Global objects
Time g_Time;
Input g_Input;
Window g_Window;


//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//
//void processInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}
//void KeyCallback()

int main() {
#pragma region initial
	if (!glfwInit()) {
		cout << "Error: khoi tao glfw that bai!" << endl;
		return ERROR_CODE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/*GLFWwindow* myWindow = glfwCreateWindow(WIDTH, HEIGHT, "Do an do hoa may tinh - Luu Van Loi ft Ninh Duc Thien, Do Manh Hung", NULL, NULL);
	if (!myWindow) {
		cout << "Error: khoi tao window that bai!" << endl;
		glfwTerminate();
		return ERROR_CODE;
	}

	glfwMakeContextCurrent(myWindow);*/

	g_Window.Initialize(WIDTH, HEIGHT, "Do an do hoa may tinh - Luu Van Loi ft Ninh Duc Thien, Do Manh Hung");
	if (!g_Window) {
		glfwTerminate();
		cout << "Error: khoi tao window that bai!" << endl;
		return ERROR_CODE;
	}
	glfwMakeContextCurrent(g_Window);

	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return EXIT_FAILURE;
	}

	// Set callbacks
	glfwSetFramebufferSizeCallback(g_Window, framebuffer_size_callback);
	glfwSetCursorPosCallback(g_Window, mouse_callback);
	glfwSetScrollCallback(g_Window, scroll_callback);

	glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/*if (!glewInit()) {
		cout << "Error: khoi tao glew that bai!" << endl;

		return ERROR_CODE;
	}*/

	/*glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);*/
	srand(time(NULL));

#pragma endregion
	MyScene myScene;
	myScene.Initialize();
	myScene.CreateScene();
	myScene.Run();
	myScene.Destroy();

	glfwTerminate();
	return SUCCESS_CODE;
}

//Callback
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}