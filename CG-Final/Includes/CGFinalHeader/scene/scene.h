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

	// 1 for plane, big number for border
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
		plane = new Plane(ResourceManager::GetShader("ShadowShader"), ResourceManager::GetTexture("wood"));
		plane->setParam(3.0f, 3.0f);
		// border
		border = new Border(ResourceManager::GetShader("ShadowShader"), ResourceManager::GetTexture("wood"));
	}

	void Draw(string shaderName, GLuint* depthMap)
	{
		plane->shader = ResourceManager::GetShader(shaderName);
		plane->depthMap = depthMap;
		border->shader = ResourceManager::GetShader(shaderName);
		border->depthMap = depthMap;

		plane->DrawBlock(2, 2);
		plane->DrawBlock(2, 5);
		plane->DrawBlock(2, 7);
		plane->DrawBlock(2, 9);
		plane->DrawBlock(2, 11);
		plane->DrawBlock(2, 13);
		plane->DrawBlock(2, 15);
		plane->DrawBlock(2, 17);
		plane->DrawBlock(17, 2);
		plane->DrawBlock(17, 5);
		plane->DrawBlock(17, 7);
		plane->DrawBlock(17, 9);
		plane->DrawBlock(17, 11);
		plane->DrawBlock(17, 13);
		plane->DrawBlock(17, 15);
		plane->DrawBlock(17, 17);
		plane->DrawBlock(5, 2);
		plane->DrawBlock(7, 2);
		plane->DrawBlock(9, 2);
		plane->DrawBlock(11, 2);
		plane->DrawBlock(13, 2);
		plane->DrawBlock(15, 2);
		plane->DrawBlock(5, 17);
		plane->DrawBlock(7, 17);
		plane->DrawBlock(9, 17);
		plane->DrawBlock(11, 17);
		plane->DrawBlock(13, 17);
		plane->DrawBlock(15, 17);

		//border->setParam(19.0f, 1.0f);
		border->DrawBlock(9, 0, 19);
		border->DrawBlock(10, 19, 19);

		//border->setParam(1.0f, 19.0f);
		border->DrawBlock(0, 10, 19, true);
		border->DrawBlock(19, 9, 19, true);

		//border->setParam(11.0f, 1.0f);
		border->DrawBlock(9, 4, 11);
		border->DrawBlock(10, 15, 11);

		//border->setParam(1.0f, 11.0f);
		border->DrawBlock(4, 10, 11, true);
		border->DrawBlock(15, 9, 11, true);
	}

	void reDraw(string shaderName, GLuint* depthMap)
	{
		plane->shader = ResourceManager::GetShader(shaderName);
		plane->depthMap = depthMap;
		border->shader = ResourceManager::GetShader(shaderName);
		border->depthMap = depthMap;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (sceneMatrix[i][j] == 1) {
					plane->DrawBlock(i, j);
				}
				else if (sceneMatrix[i][j] != 0) {
					if (sceneMatrix[i][j] > 0) {
						border->setParam(abs(sceneMatrix[i][j]), 1.0f);
						//border->DrawBlock(i, j);
					}
					else {
						border->setParam(1.0f, abs(sceneMatrix[i][j]));
						//border->DrawBlock(i, j);
					}
				}
			}
		}
	}

	void DrawTest(string shaderName, GLuint* depthMap)
	{
		plane->shader = ResourceManager::GetShader(shaderName);
		plane->depthMap = depthMap;
		border->shader = ResourceManager::GetShader(shaderName);
		border->depthMap = depthMap;
		/*plane->DrawBlock(0, 0);
		plane->DrawBlock(1, 0);
		plane->DrawBlock(0, 1);*/
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (sceneMatrix[i][j] == 1) {
					plane->DrawBlock(i, j);
				}
			}
		}
	}
};

#endif	// SCENE_H
