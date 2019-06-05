#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <iostream>

#include <CGFinalHeader/customObject/custom_object.h>

class Plane : public CustomObject
{
public:
	// Object state

	// Render state
	GLuint VAO;
	Texture2D texture;

	Plane(Shader &shader, Texture2D &texture)
		: CustomObject(shader), texture(texture)
	{ 
		this->initRenderData();
	}

	~Plane()
	{
		glDeleteVertexArrays(1, &this->VAO);
	}

	void initRenderData()
	{
		std::cout << "PlaneObject initRenderData" << std::endl;

		// Configure VAO/VBO
		GLuint VBO;
		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glBindVertexArray(this->VAO);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw()
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		this->shader.Use().SetMatrix4("model", model);

		glActiveTexture(GL_TEXTURE0);
		texture.Bind();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
};

#endif	// PLANE_H