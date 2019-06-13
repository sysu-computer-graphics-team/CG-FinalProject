#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <iostream>

#include <CGFinalHeader/resourceManager/resource_manager.h>
#include <CGFinalHeader/customObject/custom_object.h>
#include <CGFinalHeader/customObject/border.h>
#include <CGFinalHeader/customObject/plane.h>

class Scene
{
public:
	// Custom Object
	Plane* plane;
	Border* border;
	float length = 13.0f;
	float width = 3.0f;

	Scene()
	{
		this->initRenderData();
	}

	~Scene()
	{
		delete plane;
		delete border;
	}

	void initRenderData()
	{
		std::cout << "Scene initRenderData" << std::endl;

		// New Scene Object
		// plane
		plane = new Plane(ResourceManager::GetShader("BasicShader"), ResourceManager::GetTexture("container2_specular"));
		// border
		border = new Border(ResourceManager::GetShader("BasicShader"), ResourceManager::GetTexture("wood"));

	}

	void Draw()
	{
		plane->DrawSquare(0, 0);
		plane->DrawSquare(1, 0);
		border->Draw();
	}
};

#endif	// SCENE_H
