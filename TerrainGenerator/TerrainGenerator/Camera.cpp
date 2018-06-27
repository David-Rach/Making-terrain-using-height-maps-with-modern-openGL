#include "Camera.h"

CCamera::CCamera(int _width, int _height)
{
	/*Setup pos and dir*/
	m_position = glm::vec3(600, 200 , -100);
	SpeedBoost = false;
	cameraSpd = 2.5f;

	for (unsigned int i = 0; i < 256; i++)
	{
		keyArray[i] = false;
	}

	UpDirection = glm::vec3(0, 1, 0);
	CameraFront = glm::vec3(0, 0, 1);

	//make our projection matrix
	ProjectMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 1000000.0f);

	//make view matrix
	ViewMatrix = glm::lookAt
	(
		m_position,					//Camera is where now?
		m_position + CameraFront,	//Camera looks at what now?
		UpDirection					//Camera head is what way now?
	);
}

CCamera::~CCamera()
{

}

void CCamera::resize(int _width, int _height)
{
	//make our projection matrix
	ProjectMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 1000000.0f);

	//make view matrix
	ViewMatrix = glm::lookAt
	(
		m_position,					//Camera is where now?
		m_position + CameraFront,	//Camera looks at what now?
		UpDirection					//Camera head is what way now?
	);
}

void CCamera::update(float deltaTime)
{
	//make view matrix
	ViewMatrix = glm::lookAt
	(
		m_position,					//Camera is where now?
		m_position + CameraFront,	//Camera looks at what now?
		UpDirection					//Camera head is what way now?
	);

	float deltaCamSpd = cameraSpd * deltaTime;

	//Lets handle movement
	if (keyArray['w'] == true)
	{
		m_position += deltaCamSpd * CameraFront;
	}
	else if (keyArray['s'] == true)
	{
		m_position += -deltaCamSpd * CameraFront;
	}

	if (keyArray['a'] == true)
	{
		m_position += -deltaCamSpd * glm::normalize(glm::cross(CameraFront, UpDirection));
	}
	else if (keyArray['d'] == true)
	{
		m_position += deltaCamSpd * glm::normalize(glm::cross(CameraFront, UpDirection));
	}

	if (keyArray['e'] == true)
	{
		m_position += deltaCamSpd * UpDirection;
	}
	else if (keyArray['q'] == true)
	{
		m_position += -deltaCamSpd * UpDirection;
	}

	if (SpeedBoost)
	{
		cameraSpd = 200.0f;
	}
	else
	{
		cameraSpd = 20.0f;
	}
}