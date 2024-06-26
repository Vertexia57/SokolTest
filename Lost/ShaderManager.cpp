#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (Shader* shader : m_Shaders)
		delete shader;
}

ShaderID ShaderManager::addShader(Shader* shader)
{
	ShaderID assignedID = m_Shaders.size();
	shader->id = assignedID;
	m_Shaders.push_back(shader);
	m_ShaderMap[shader->name] = assignedID;
	return assignedID;
}

ShaderID ShaderManager::createShader(const ShaderLoadStruct& loadStruct)
{
	Shader* shader = new Shader();
	shader->loadShader(loadStruct.vertexFileName, loadStruct.fragmentFileName, loadStruct.shaderName);
	return addShader(shader);
}

Shader* ShaderManager::getShader(ShaderID id)
{
	return m_Shaders[id];
}

Shader* ShaderManager::getShader(std::string id)
{
	return m_Shaders[m_ShaderMap[id]];
}

namespace lost
{

	ShaderManager* shaderManager = nullptr;

	void createShaderManager()
	{
		shaderManager = new ShaderManager();
		loadShader("Shaders/vertex.vert", "Shaders/fragment.frag", "base");
	}

	void destroyShaderManager()
	{
		delete shaderManager;
	}

	ShaderID loadShader(const char* vertexShaderLoc, const char* fragmentShaderLoc, const char* shaderName)
	{
		ShaderLoadStruct loadStruct = {};
		loadStruct.vertexFileName = vertexShaderLoc;
		loadStruct.fragmentFileName = fragmentShaderLoc;
		loadStruct.shaderName = shaderName;
		return shaderManager->createShader(loadStruct);
	}

	ShaderID addShader(Shader* shader)
	{
		return shaderManager->addShader(shader);
	}

	Shader* getShader(ShaderID id)
	{
		return shaderManager->getShader(id);
	}

	Shader* getShader(std::string id)
	{
		return shaderManager->getShader(id);
	}

	void bindShader(Shader* shader)
	{
		shader->bindShader();
		shaderManager->currentShader = shader->id;
	}

	void bindShader(ShaderID shaderID)
	{
		shaderManager->getShader(shaderID)->bindShader();
		shaderManager->currentShader = shaderID;
	}

	void bindShader(std::string shaderName)
	{
		Shader* shader = shaderManager->getShader(shaderName);
		shader->bindShader();
		shaderManager->currentShader = shader->id;
	}

	void resetShader()
	{
		bindShader(ShaderID{ 0 });
		shaderManager->currentShader = 0;
	}

	void unbindShader()
	{
		sgp_reset_pipeline();
		shaderManager->currentShader = 0;
	}
}