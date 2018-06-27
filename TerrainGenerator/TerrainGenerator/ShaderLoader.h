#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <string>
#include <fstream>
#include <vector>

#include <glew.h>
#include <glfw3.h>

class CShaderLoader
{
public:
	CShaderLoader();
	~CShaderLoader();

	//load the shaders
	GLuint loadShaders(const char * _vertexShader, const char * _fragmentShader);

private:

};


#endif