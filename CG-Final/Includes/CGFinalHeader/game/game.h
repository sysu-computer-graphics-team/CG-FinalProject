#pragma once
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <CGFinalHeader/camera/camera.h>
#include <CGFinalHeader/mytext/mytext.h>
// Represent the current state of the game
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,		// not implement
	GAME_OVER
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and managebility
class Game
{
public:
	// Game state
	GameState		State;
	// use Phong or Blinn for lighting?
	GLboolean		isBlinn;	
	Camera			*camera;
	// glfw window configure
	GLuint			Width, Height;
	GLuint depthMapFBO, depthMap;
	GLuint planeVAO;
	float Yaw = -90;
	myText mytext;
	std::string str;
	glm::vec3 carPos = glm::vec3(0, 0, 0);
	glm::vec3 axis = glm::vec3(0,1,0);
	glm::vec3 carfront = glm::vec3(0.0f, 0.0f, -1.0f);
	// Constructor/Destructor
	Game(GLuint width, GLuint height, Camera *camera);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLFWwindow *window, Camera_Movement direction, glm::vec3 frontOfCar, glm::vec3 upOfCar, float deltaTime);
	void Update();
	void Render();
	glm::vec3 getFrontOfCar();
};

#endif // GAME_H

