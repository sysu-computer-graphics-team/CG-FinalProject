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

	// 1 for plane, 2 for border
	int sceneMatrix[20][20] = {
	{0,0,0,0,0,0,0,0,0,0,-19,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,-11,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{19,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,19},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,-11,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,-19,0,0,0,0,0,0,0,0,0,0}
	};

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
		plane->setParam(3.0f, 3.0f);
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (sceneMatrix[i][j] == 1) {
					plane->DrawBlock(i, j);
				}
				else if (sceneMatrix[i][j] != 0) {
					if (sceneMatrix[i][j] > 0) {
						border->setParam(abs(sceneMatrix[i][j]), 1.0f);
						border->DrawBlock(i, j);
					}
					else {
						border->setParam(1.0f, abs(sceneMatrix[i][j]));
						border->DrawBlock(i, j);
					}
				}
			}
		}
	}
};

#endif	// SCENE_H
