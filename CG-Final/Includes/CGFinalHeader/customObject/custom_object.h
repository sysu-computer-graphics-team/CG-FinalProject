#pragma once
#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include <CGFinalHeader/texture/texture.h>
#include <CGFinalHeader/shader/shader.h>
// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class CustomObject
{
public:
	// Object state

	// Render state
	Shader shader;
	// Constructor(s)
	CustomObject(Shader &shader);
	~CustomObject();
	// Initializes and configures the VAO's buffer and vertex attributes
	virtual void initRenderData() = 0;
	// Draw GameObject
	virtual void Draw() = 0;
};

#endif	// CUSTOMOBJECT_H