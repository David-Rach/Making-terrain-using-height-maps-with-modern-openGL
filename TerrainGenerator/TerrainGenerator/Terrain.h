#ifndef TERRAIN_H
#define TERRAIN_H

/*Local Includes*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glew.h>
#include <glfw3.h>

#include "Camera.h"

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

	void ReadHeightmap(std::string filepath);
	void GenerateVertices();
	void BuildTerrain();

	/*Render calls*/
	void Render(CCamera* camera, GLuint Shader);

private:
	enum {POSITION, NUMBER_OF_BUFFERS};

	/*Heightmap Information*/
	std::vector<float> RawHeightmapInformation;
	unsigned int iNumOfVertices;
	unsigned int iNumOfRows;
	unsigned int iNumOfCols;

	/*Vertices information*/
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<unsigned int> Indices;

	/*World Information*/
	glm::vec3 m_position;
	glm::vec3 m_scale;

	/*OpenGL Information*/
	GLuint VertexArrayObject;
	GLuint BufferArrayObject[NUMBER_OF_BUFFERS];

	glm::mat4 m_modelMatrix;
	glm::mat4 m_positionMatrix;
	glm::mat4 m_scaleMatrix;
};

#endif