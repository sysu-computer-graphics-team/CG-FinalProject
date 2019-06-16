#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H

#include <iostream>
#include <CGFinalHeader/customObject/custom_object.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
using namespace std;

class windowsObject : public CustomObject
{
public:
	// Object state
	glm::vec3 cameraPosition;
	// Render state
	GLuint VAO;
	Texture2D texture;
	GLuint *depthMap = nullptr;
	vector<glm::vec3> windows
	{
		glm::vec3(10.0f, 0.0f, -10.0f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
	windowsObject(Shader &shader, Texture2D &texture)
		: CustomObject(shader), texture(texture)
	{
		this->initRenderData();
	}

	~windowsObject()
	{
		glDeleteVertexArrays(1, &this->VAO);
	}

	void initRenderData()
	{
		std::cout << "WindowsObject initRenderData" << std::endl;
		
		// Configure VAO/VBO
		float transparentVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		// transparent VAO
		GLuint VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);
	}

	void Draw()
	{
		// sort the transparent windows before rendering
		map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++){
			float distance = glm::length(cameraPosition - windows[i]);
			sorted[distance] = windows[i];
		}
		// render
		
			glm::mat4 model = glm::mat4(1.0f);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		if (this->depthMap != nullptr)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, *depthMap);
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			shader.Use().SetMatrix4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	}
};


#endif	// WINDOWS_H
