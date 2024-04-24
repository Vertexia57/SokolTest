#pragma once
#include "../SokolReference.h"

#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

typedef uint32_t ShaderID;

static std::string read_UTF8_file(const char* fileName)
{
	// Create the Input File stream
	std::ifstream in(fileName, std::ios::in | std::ios::binary);
	// Check if it loaded
	if (in)
	{
		std::string loadedString = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		loadedString.erase(remove(loadedString.begin(), loadedString.end(), '\r'), loadedString.end());
		return(loadedString);
	}
	// Failed passed here
	fprintf(stderr, " [func: read_UTF8_file] Failed to load file with file name: %s\n", fileName);
	throw(errno);
}

class Shader
{
public:
	Shader();
	~Shader();

	// Loads the shader data and creates the shader
	void loadShader(const char* vertexShaderLoc, const char* fragmentShaderLoc, const char* _name);

	// Binds the shader pipeline
	void bindShader();

	sg_shader shader;
	sg_pipeline pipeline;
	std::string name;
	ShaderID id = 0;

private:

	// Creates the shader description
	sg_shader_desc* m_CreateDescriptor(const char* vertexShaderCode, const char* fragmentShaderCode);
	sg_shader_desc* m_ShaderDescriptor = nullptr;

	// Creates the shader after loading the data
	void m_CreateShader();

	std::string vertexShaderLoc;
	std::string fragmentShaderLoc;
};