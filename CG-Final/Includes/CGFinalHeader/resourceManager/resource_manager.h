#pragma once
#ifndef RESOURCE_MANAGER_H
#define	RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include <CGFinalHeader/texture/texture.h>
#include <CGFinalHeader/shader/shader.h>
#include <CGFinalHeader/model/model.h>

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, Model> Models;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// Retrieves a stored shader
	static Shader &GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// Retrieves a stored texture
	static Texture2D &GetTexture(std::string name);
	// Loads (and generates) a model from file
	static Model LoadModel(const GLchar *file, std::string name);
	// Retrieves a stored model
	static Model *GetModel(std::string name);
	// Load skybox texture
	static unsigned int LoadCubemap(vector<std::string> faces);

	// Properly de-allocates all loaded resources
	static void Clear();
private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }
	// Loads and generates a shader from file
	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// Loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};


#endif // RESOURCE_MANAGER_H