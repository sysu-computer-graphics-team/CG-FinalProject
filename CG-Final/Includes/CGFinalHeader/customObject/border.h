#pragma once
#ifndef BORDER_H
#define BORDER_H

#include <iostream>

#include <CGFinalHeader/customObject/custom_object.h>

class Border : public CustomObject
{
public:
	// Object state

	// Render state
	GLuint VAO;
	Texture2D texture;
	float length = 1.0f;
	float width = 1.0f;

	Border(Shader &shader, Texture2D &texture)
		: CustomObject(shader), texture(texture)
	{
		this->initRenderData();
	}

	~Border()
	{
		glDeleteVertexArrays(1, &this->VAO);
	}

	void setParam(int l, int w)
	{
		length = l;
		width = w;
		this->initRenderData();
	}

	void initRenderData()
	{
		std::cout << "BorderObject initRenderData" << std::endl;

		// Configure VAO/VBO
		GLuint VBO;
		float borderVertices[] = {
			// back face
			-length, -1.0f, -width,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 length,  1.0f, -width,  0.0f,  0.0f, -1.0f, length, width, // top-right
			 length, -1.0f, -width,  0.0f,  0.0f, -1.0f, length, 0.0f, // bottom-right         
			 length,  1.0f, -width,  0.0f,  0.0f, -1.0f, length, width, // top-right
			-length, -1.0f, -width,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-length,  1.0f, -width,  0.0f,  0.0f, -1.0f, 0.0f, width, // top-left
			// front face
			-length, -1.0f,  width,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 length, -1.0f,  width,  0.0f,  0.0f,  1.0f, length, 0.0f, // bottom-right
			 length,  1.0f,  width,  0.0f,  0.0f,  1.0f, length, width, // top-right
			 length,  1.0f,  width,  0.0f,  0.0f,  1.0f, length, width, // top-right
			-length,  1.0f,  width,  0.0f,  0.0f,  1.0f, 0.0f, width, // top-left
			-length, -1.0f,  width,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-length,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, length, 0.0f, // top-right
			-length,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, length, width, // top-left
			-length, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, width, // bottom-left
			-length, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, width, // bottom-left
			-length, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-length,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, length, 0.0f, // top-right
			// right face
			 length,  1.0f,  width,  1.0f,  0.0f,  0.0f, length, 0.0f, // top-left
			 length, -1.0f, -width,  1.0f,  0.0f,  0.0f, 0.0f, width, // bottom-right
			 length,  1.0f, -width,  1.0f,  0.0f,  0.0f, length, width, // top-right         
			 length, -1.0f, -width,  1.0f,  0.0f,  0.0f, 0.0f, width, // bottom-right
			 length,  1.0f,  width,  1.0f,  0.0f,  0.0f, length, 0.0f, // top-left
			 length, -1.0f,  width,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-length, -1.0f, -width,  0.0f, -1.0f,  0.0f, 0.0f, width, // top-right
			 length, -1.0f, -width,  0.0f, -1.0f,  0.0f, length, width, // top-left
			 length, -1.0f,  width,  0.0f, -1.0f,  0.0f, length, 0.0f, // bottom-left
			 length, -1.0f,  width,  0.0f, -1.0f,  0.0f, length, 0.0f, // bottom-left
			-length, -1.0f,  width,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-length, -1.0f, -width,  0.0f, -1.0f,  0.0f, 0.0f, width, // top-right
			// top face
			-length,  1.0f, -width,  0.0f,  1.0f,  0.0f, 0.0f, width, // top-left
			 length,  1.0f , width,  0.0f,  1.0f,  0.0f, length, 0.0f, // bottom-right
			 length,  1.0f, -width,  0.0f,  1.0f,  0.0f, length, width, // top-right     
			 length,  1.0f,  width,  0.0f,  1.0f,  0.0f, length, 0.0f, // bottom-right
			-length,  1.0f, -width,  0.0f,  1.0f,  0.0f, 0.0f, width, // top-left
			-length,  1.0f,  width,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left    
		};

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(borderVertices), borderVertices, GL_STATIC_DRAW);
		// link vertex attributes
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

	void DrawBlock(float x, float z)
	{
		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		glBindVertexArray(VAO);

		float blockLength = 2.0f;
		float x_offset = length * 2.0f * x;
		float z_offset = length * 2.0f * z;

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(x_offset, 0.0f, z_offset));
		this->shader.Use().SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

	void Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		glBindVertexArray(VAO);

		float blockLength = 2.0f;

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(2 * blockLength, 0.0f, 2 * blockLength));
		this->shader.Use().SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.5f));
		model = glm::scale(model, glm::vec3(11.0f, 0.5f, 0.5f));
		this->shader.Use().SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.5f, 0.5f));
		this->shader.Use().SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.5f, 0.5f));
		this->shader.Use().SetMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		glBindVertexArray(0);
	}
};


#endif	// BORDER_H