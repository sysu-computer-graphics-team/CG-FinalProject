#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>

#include <CGFinalHeader/customObject/custom_object.h>

struct myParticle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	myParticle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(10.0f) { }
};

class ParticleGenerator {
public:
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
		: shader(shader), texture(texture), amount(amount)
	{
		this->init();
	}
	void Update(GLfloat dt, GLuint newParticles, glm::vec2 offset, glm::vec2 position, glm::vec2 velocity)
	{
		// Add new particles 
		for (GLuint i = 0; i < newParticles; ++i)
		{
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->particles[unusedParticle], offset, position, velocity);
		}
		// Update all particles
		for (GLuint i = 0; i < this->amount; ++i)
		{
			myParticle& p = this->particles[i];
			p.Life -= dt; // reduce life
			if (p.Life > 0.0f)
			{	// particle is alive, thus update
				p.Position -= p.Velocity * dt;
				p.Color.a -= dt * 2.5;
			}
		}
	}
	void Draw()
	{
		// Use additive blending to give it a 'glow' effect
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		this->shader.Use();
		for (myParticle particle : this->particles)
		{
			if (particle.Life > 0.0f)
			{
				this->shader.SetVector2f("offset", particle.Position);
				this->shader.SetVector4f("color", particle.Color);
				this->texture.Bind();
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
		}
		// Don't forget to reset to default blending mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
private:
	std::vector<myParticle> particles;
	GLuint amount;
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	GLuint lastUsedParticle = 0;
	void init() {
		// Set up mesh and attribute properties
		GLuint VBO;
		GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(this->VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		// Create this->amount default particle instances
		for (GLuint i = 0; i < this->amount; ++i)
			this->particles.push_back(myParticle());
	}
	GLuint firstUnusedParticle()
	{
		// First search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		lastUsedParticle = 0;
		return 0;
	}
	void respawnParticle(myParticle& particle, glm::vec2 offset, glm::vec2 position, glm::vec2 velocity)
	{
		GLfloat random = ((rand() % 100) - 50) / 10.0f;
		GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
		//particle.Position = position + random + offset;
		particle.Position = glm::vec2(10.0f, 10.0f);
		particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
		particle.Life = 10.0f;
		particle.Velocity = velocity * 0.1f;
	}
};

#endif	// PARTICLE_H