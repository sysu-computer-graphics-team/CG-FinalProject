#pragma once
#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include <CGFinalHeader/texture/texture.h>
#include <CGFinalHeader/shader/shader.h>
#include <CGFinalHeader/model/model.h>

class ModelObject
{
public:
	// Render state
	Shader shader;
	Model *model;
	// Constructor(s)
	ModelObject(Shader &shader, Model *model) : shader(shader), model(model)
	{

	}
	~ModelObject() {

	}
	// Draw ModelObject
	void Draw()
	{
		model->Draw(shader);
	}
};

#endif	// MODELOBJECT_H