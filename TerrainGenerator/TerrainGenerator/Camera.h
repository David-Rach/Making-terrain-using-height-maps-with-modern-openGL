#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class CCamera
{
public:
	CCamera(int _width, int _height);
	~CCamera();

	/*Public matrices*/
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectMatrix;

	/*Camera functions*/
	void resize(int _width, int _height);
	void update(float deltaTime);

	glm::vec3 UpDirection;
	glm::vec3 CameraFront;
	glm::vec3 CameraRight;
	glm::vec3 m_position;

	float cameraSpd;
	bool SpeedBoost;
	bool keyArray[256];

private:
};
#endif