#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <CGFinalHeader/camera/camera.h>
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

	// Constructor/Destructor
	Game(GLuint width, GLuint height, Camera *camera);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLFWwindow *window, GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};

#endif // GAME_H

