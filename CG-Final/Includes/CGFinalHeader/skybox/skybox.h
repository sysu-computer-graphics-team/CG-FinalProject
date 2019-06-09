#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>

#include <CGFinalHeader/customObject/custom_object.h>

class Skybox : public CustomObject
{
public:
	// Object state

	// Render state
	GLuint VAO;
	unsigned int cubemapTexture;
	const float skyLength = 50.0f;

	Skybox(Shader &shader, unsigned int &texture)
		: CustomObject(shader), cubemapTexture(texture)
	{
		this->initRenderData();
	}

	~Skybox()
	{
		glDeleteVertexArrays(1, &this->VAO);
	}

	void initRenderData()
	{
		std::cout << "Skybox initRenderData" << std::endl;

		// Configure VAO/VBO
		GLuint VBO;
		float SkyboxVertices[] = {
			-skyLength,  skyLength, -skyLength,
			-skyLength, -skyLength, -skyLength,
			 skyLength, -skyLength, -skyLength,
			 skyLength, -skyLength, -skyLength,
			 skyLength,  skyLength, -skyLength,
			-skyLength,  skyLength, -skyLength,

			-skyLength, -skyLength,  skyLength,
			-skyLength, -skyLength, -skyLength,
			-skyLength,  skyLength, -skyLength,
			-skyLength,  skyLength, -skyLength,
			-skyLength,  skyLength,  skyLength,
			-skyLength, -skyLength,  skyLength,

			 skyLength, -skyLength, -skyLength,
			 skyLength, -skyLength,  skyLength,
			 skyLength,  skyLength,  skyLength,
			 skyLength,  skyLength,  skyLength,
			 skyLength,  skyLength, -skyLength,
			 skyLength, -skyLength, -skyLength,

			-skyLength, -skyLength,  skyLength,
			-skyLength,  skyLength,  skyLength,
			 skyLength,  skyLength,  skyLength,
			 skyLength,  skyLength,  skyLength,
			 skyLength, -skyLength,  skyLength,
			-skyLength, -skyLength,  skyLength,

			-skyLength,  skyLength, -skyLength,
			 skyLength,  skyLength, -skyLength,
			 skyLength,  skyLength,  skyLength,
			 skyLength,  skyLength,  skyLength,
			-skyLength,  skyLength,  skyLength,
			-skyLength,  skyLength, -skyLength,

			-skyLength, -skyLength, -skyLength,
			-skyLength, -skyLength,  skyLength,
			 skyLength, -skyLength, -skyLength,
			 skyLength, -skyLength, -skyLength,
			-skyLength, -skyLength,  skyLength,
			 skyLength, -skyLength,  skyLength
		};

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), SkyboxVertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(this->VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		shader.Use();
		shader.SetInteger("skybox", 0);
	}

	void Draw()
	{
		glm::mat4 model = glm::mat4(1.0f);

		glDepthFunc(GL_LEQUAL);
		shader.Use();
		// skybox cube
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDepthMask(GL_TRUE);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

};


#endif	// SKYBOX_H