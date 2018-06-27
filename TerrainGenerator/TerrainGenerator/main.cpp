/*External Includes*/
#include <iostream>
#include <vld.h>
#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>
#include <time.h>

/*Local Includes*/
#include "Camera.h"
#include "Terrain.h"
#include "ShaderLoader.h"

using namespace std;

GLFWwindow* MAIN_WINDOW;
CCamera* FreeFlyingCamera;
CTerrain* Landscape;

GLuint SHADER_DEFAULT;

bool isFullScreen;

float deltaTime;
float lastFrame;

float lastXpos;
float lastYpos;

float Yaw;
float Pitch;

/*Used to retrieve the last GLFW error*/
void Callstack_error(int error, const char* description)
{
	cout << stderr, "Error: %s\n", description;
}

/*Keyboard Control scheme*/
void KeyboardCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	/*Close the window*/
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}

	/*Forwards*/
	if (_key == GLFW_KEY_W && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['w'] = true;
	}
	else if (_key == GLFW_KEY_W && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['w'] = false;
	}

	/*Backwards*/
	if (_key == GLFW_KEY_S && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['s'] = true;
	}
	else if (_key == GLFW_KEY_S && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['s'] = false;
	}

	/*Left*/
	if (_key == GLFW_KEY_A && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['a'] = true;
	}
	else if (_key == GLFW_KEY_A && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['a'] = false;
	}

	/*Right*/
	if (_key == GLFW_KEY_D && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['d'] = true;
	}
	else if (_key == GLFW_KEY_D && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['d'] = false;
	}


	/*Up*/
	if (_key == GLFW_KEY_E && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['e'] = true;
	}
	else if (_key == GLFW_KEY_E && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['e'] = false;
	}


	/*down*/
	if (_key == GLFW_KEY_Q && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->keyArray['q'] = true;
	}
	else if (_key == GLFW_KEY_Q && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->keyArray['q'] = false;
	}

	/*Boost*/
	if (_key == GLFW_KEY_LEFT_SHIFT && _action == GLFW_PRESS)
	{
		FreeFlyingCamera->SpeedBoost = true;
	}
	else if (_key == GLFW_KEY_LEFT_SHIFT && _action == GLFW_RELEASE)
	{
		FreeFlyingCamera->SpeedBoost = false;
	}

	/*Fullscreen*/
	if (_key == GLFW_KEY_F && _action == GLFW_PRESS)
	{
		if (!isFullScreen)
		{
			float width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
			float height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
			isFullScreen = true;

			glfwSetWindowMonitor(MAIN_WINDOW, glfwGetWindowMonitor(MAIN_WINDOW), 0, 0, width, height, 60);
		}
		else
		{
			glfwSetWindowMonitor(MAIN_WINDOW, NULL, 100, 100, 1200, 800, 60);
			isFullScreen = false;
		}

	}
}

/*Mouse Control scheme*/
void MouseCallback(GLFWwindow* _window, double _xPos, double _yPos)
{
	/*Offsets*/
	float xOffset = _xPos - lastXpos;
	float yOffset = lastYpos - _yPos;

	lastXpos = _xPos;
	lastYpos = _yPos;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	Yaw += xOffset;
	Pitch += yOffset;
	
	//This prevents strange angle problems when you look staight up or down
	if (Pitch > 89.0f)
	{
		Pitch = 89.0f;
	}
	
	if (Pitch < -89.0f)
	{
		Pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	FreeFlyingCamera->CameraFront = glm::normalize(front);

}

/*Windows resize callback*/
void WindowResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	FreeFlyingCamera->resize(_width, _height);
	glViewport(0, 0, _width, _height);
}

/*Initiates the core parts of the program*/
bool InitiateSystem()
{
	/*Initialize GLFW library*/
	if (!glfwInit())
	{
		//returns a nope to the mainframe
		cout << "ERROR!!" << endl;
		cout << "Aww. That's a nope on the GLFW initialize. Eheh... Get help." << endl;
		system("pause");
		glfwTerminate();
		return false;
	}

	/*Create a window*/
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	MAIN_WINDOW = glfwCreateWindow(1200, 800, "Terrain Generation Using Modern OpenGL", NULL, NULL);
	if (!MAIN_WINDOW)
	{
		//If the window fails to be created, there is quite literally no point in continuing
		cout << "ERROR!!" << endl;
		cout << "Window failed to open. Did you open the curtain?" << endl;
		system("pause");
		glfwTerminate();
		return false;
	}

	/*make this window the current context*/
	glfwMakeContextCurrent(MAIN_WINDOW);

	/*initailize GLEW library*/
	GLenum glewChk = glewInit();
	if (GLEW_OK != glewChk)
	{
		cout << "Failed to initiate GLEW library. Something is seriously wrong" << endl;
		cout << stderr, "Error: %s\n", glewGetErrorString(GLEW_VERSION);
		system("pause");
		glfwTerminate();
		return false;
	}

	/*Initialize the time seed*/
	srand(time(NULL));

	/*Set call backs*/
	glfwSetErrorCallback(Callstack_error);
	glfwSetKeyCallback(MAIN_WINDOW,KeyboardCallback);
	glfwSetCursorPosCallback(MAIN_WINDOW, MouseCallback);
	glfwSetWindowSizeCallback(MAIN_WINDOW, WindowResizeCallback);
	glfwSetInputMode(MAIN_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

/*inits for everything not system related*/
bool OnGamePlayBegin()
{
	/*Shaders*/
	CShaderLoader ShaderLoader;
	SHADER_DEFAULT = ShaderLoader.loadShaders("Assets/Shaders/Default.vs","Assets/Shaders/Default.fs");

	/*Camera*/
	FreeFlyingCamera = new CCamera(1200,800);	//Should pull from glfw :P

	/*Landscape*/
	Landscape = new CTerrain();
	Landscape->ReadHeightmap("Assets/Heightmaps/Heightmap_tut.raw");
	Landscape->GenerateVertices();
	Landscape->BuildTerrain();

	/*Mouse setup*/
	lastXpos = 600.0f;
	lastYpos = 400.0f;

	Yaw = 0.0f;
	Pitch = 0.0f;
	isFullScreen = false;


	return true;
}

/*A clean shut down. I try to manage memory leaks the best I can...*/
bool ShutDownSystem()
{
	delete Landscape;
	delete FreeFlyingCamera;
	glfwTerminate();
	return true;
}

/*The loop where logic goes*/
void MainLogicLoop()
{
	/*Cameras*/
	FreeFlyingCamera->update(deltaTime);

	/*Deltatime*/
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

/*The loop where rendering objects go*/
void MainRenderLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	Landscape->Render(FreeFlyingCamera, SHADER_DEFAULT);

	glfwSwapBuffers(MAIN_WINDOW);
	glfwPollEvents();
}

/*Entry point*/
int main()
{
	/*Start Everything*/
	if (!InitiateSystem())
	{
		return 0;
	}

	/*Setup level*/
	if (!OnGamePlayBegin())
	{
		return 0;
	}

	/*The Main Loop*/
	while (!glfwWindowShouldClose(MAIN_WINDOW))
	{
		MainLogicLoop();
		MainRenderLoop();
	}

	/*Stop everything*/
	if (!ShutDownSystem())
	{
		return 0;
	}

	return 1;
}