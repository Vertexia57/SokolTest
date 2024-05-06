#pragma once
#include "Shader.h"
#include <vector>
#include <map>

struct ShaderLoadStruct
{
	const char* vertexFileName;
	const char* fragmentFileName;
	const char* shaderName;
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	// Adds the shader to the manager, this requires the shader is already loaded and has a name
	ShaderID addShader(Shader* shader);
	// Loads, creates and adds a shader to the manager
	ShaderID createShader(const ShaderLoadStruct& loadStruct);

	// Returns the shader with the shader id specified
	Shader* getShader(ShaderID id);
	// Returns the shader with the shader name specified
	Shader* getShader(std::string id);

	ShaderID currentShader = 0;
private:
	uint32_t m_ShadersAllocated = 0; // This is used to assign TextureID

	std::vector<Shader*> m_Shaders;
	std::map<std::string, ShaderID> m_ShaderMap;
};

namespace lost
{

	extern ShaderManager* shaderManager;

	extern void createShaderManager();
	extern void destroyShaderManager();

	// Loads, creates and adds a shader to the manager
	extern ShaderID loadShader(const char* vertexShaderLoc, const char* fragmentShaderLoc, const char* shaderName);
	// Adds the shader to the manager, this requires the shader is already loaded and has a name
	extern ShaderID addShader(Shader* shader);

	// Returns the shader with the shader id specified
	extern Shader* getShader(ShaderID id);
	// Returns the shader with the shader name specified
	extern Shader* getShader(std::string id);

	// Binds the shader pipeline
	extern void bindShader(Shader* shader);
	// Binds the shader pipeline
	extern void bindShader(ShaderID shaderID);
	// Binds the shader pipeline
	extern void bindShader(std::string shaderName);

	// Resets the current shader pipeline
	extern void resetShader();
	// Unbinds the current shader pipeline
	extern void unbindShader();

}