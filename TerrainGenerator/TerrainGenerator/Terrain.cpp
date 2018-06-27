#include "Terrain.h"

CTerrain::CTerrain()
{
	/*Init Values*/
	iNumOfCols = 1024;
	iNumOfRows = 1024;
	iNumOfVertices = iNumOfCols * iNumOfRows;

	m_position = glm::vec3(0, 0, 0);
	m_scale = glm::vec3(1, 1, 1);
}

CTerrain::~CTerrain()
{
}

/*Reads the heightmap and pulls out all values*/
void CTerrain::ReadHeightmap(std::string filepath)
{
	/*Raw information*/
	std::vector<unsigned char> data(iNumOfVertices);

	/*Open the file*/
	std::ifstream infile;
	infile.open(filepath, std::ios_base::binary);

	if (infile)
	{
		std::cout << "Heightmap file successfully opened!" << std::endl;

		/*Reads the file*/
		infile.read((char*)&data[0], (std::streamsize)data.size());
		infile.close();
	}
	else
	{
		std::cout << "Heightmap file could not be found" << std::endl;
	}

	/*Conversion into floats*/
	RawHeightmapInformation.resize(iNumOfRows * iNumOfCols, 0.0f);
	for (unsigned int i = 0; i < iNumOfRows * iNumOfCols; i++)
	{
		RawHeightmapInformation[i] = (float)data[i];	//Quite possibly may need to change height scaling...?
	}
}

/*Generates a grid with the values read from the heightmap*/
void CTerrain::GenerateVertices()
{
	for (unsigned int x = 0; x < iNumOfRows; x++)
	{
		for (unsigned z = 0; z < iNumOfCols; z++)
		{
			/*Generate a point with XYZ*/
			Vertices.push_back(glm::vec3((float)x, RawHeightmapInformation[x * iNumOfRows + z], (float)z));
		}
	}
}

/*This is what allows us to draw the terrain*/
void CTerrain::BuildTerrain()
{
	/*The Vertex Array*/
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	/*Create the buffers*/
	glGenBuffers(NUMBER_OF_BUFFERS, BufferArrayObject);

	/*The Position Buffer*/
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObject[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*Unbind the vertex array*/
	glBindVertexArray(0);
}

void CTerrain::Render(CCamera* camera, GLuint Shader)
{
	/*Switches to passed in shader*/
	glUseProgram(Shader);

	/*Make the matrices first...*/
	m_positionMatrix = glm::translate(m_position);
	m_scaleMatrix = glm::scale(m_scale);
	m_modelMatrix = m_positionMatrix * m_scaleMatrix;	//Where is the rotation you ask? I hate rotations!

	/*Hand over the matrics from the camera and model*/
	glUniformMatrix4fv(glGetUniformLocation(Shader, "Model"), 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "View"), 1, GL_FALSE, &camera->ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "Projection"), 1, GL_FALSE, &camera->ProjectMatrix[0][0]);
	glm::vec3 Colorpicker(5.0f,14.0f,0.0f);
	Colorpicker = glm::normalize(Colorpicker);
	glUniform3f(glGetUniformLocation(Shader, "color"), Colorpicker.x, Colorpicker.y, Colorpicker.z);

	glBindVertexArray(VertexArrayObject);

	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, 0, Vertices.size());

	glBindVertexArray(0);
}
