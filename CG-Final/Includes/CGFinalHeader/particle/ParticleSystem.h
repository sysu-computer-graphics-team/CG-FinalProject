#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <cmath>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <CGFinalHeader/resourceManager/resource_manager.h>
using std::vector;

struct Particle {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec2 rotateAngle;
	float rotateSpeed;
	float lifetime;
	float scale;
	float dec;

	Particle()
		: position(0.0f), velocity(0.0f), color(1.0f), lifetime(1.0f), scale(1.0f), acceleration(1.0f), dec(1.0f), rotateAngle(0.0f), rotateSpeed(0.0f) { }
};


class ParticleSystem {
private:
	glm::vec3 particlesGenPos;
	float range_x, range_z, skyHeight;
	int number;

	const float DT_SCALE = 100.0f;
	const float LIFE_TIME = 100.0f;
	const float VELOCITY_X_MAX = 0.01f;
	const float VELOCITY_Y_MIN = 0.05f;
	const float VELOCITY_Y_MAX = 0.1f;
	const float ROTATE_ANGLE_MAX = 360.0f;
	const float ROTATE_VELOCITY = 0.001f;
	const float G_ACCE = 0.000005f;
	const float DEC_MAX = 0.05f;
	const float SCALE_MIN = 0.05f;
	const float SCALE_MAX = 0.1f;
	vector<Particle> particles;

	float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec, rx, ry, rv;
	float r, g, b, a;
	glm::vec3 position, velocity, acceleration, scale;

	unsigned int VAO;
	//unsigned int snowTexture;
	Texture2D snowTexture;
public:
	ParticleSystem(int number) {
		this->number = number;
		initParticleSystem();
	}
	ParticleSystem(int number, glm::vec3 genPosition, float range_x, float range_z, float skyHeight) {
		this->number = number;
		this->particlesGenPos = genPosition;
		this->range_x = range_x;
		this->range_z = range_z;
		this->skyHeight = skyHeight;

		initParticleSystem();
	}
	void initParticleSystem() {
		snowTexture = ResourceManager::GetTexture("snow");
		particles.resize(number);
		initParticleVAO();

		for (Particle& particle : particles) {
			respawnParticle(particle);
		}

	}
	void initParticleVAO() {
		GLuint VBO;
		GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}

	void respawnParticle(Particle& particle) {
		//初始化颜色（白色）
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		a = 1.0f;
		particle.color = glm::vec4(r, g, b, a);

		//初始化坐标
		x = range_x * (rand() / float(RAND_MAX)) - range_x / 2;
		z = range_z * (rand() / float(RAND_MAX)) - range_z / 2;
		y = skyHeight;
		particle.position = glm::vec3(x, y, z);

		//初始化速度
		vx = rand() / float(RAND_MAX) * VELOCITY_X_MAX;
		vy = rand() / float(RAND_MAX) * (VELOCITY_Y_MAX - VELOCITY_Y_MIN) + VELOCITY_Y_MIN;
		vz = 0;
		particle.velocity = glm::vec3(vx, vy, vz);


		//旋转角度
		rx = (rand() / float(RAND_MAX)) * ROTATE_ANGLE_MAX;
		ry = (rand() / float(RAND_MAX)) * ROTATE_ANGLE_MAX;
		//旋转速度
		rv = rand() / float(RAND_MAX) * ROTATE_VELOCITY;

		particle.rotateAngle = glm::vec2(rx, ry);
		particle.rotateSpeed = rv;

		//初始化加速度
		ax = 0;
		ay = G_ACCE;
		az = 0;
		particle.acceleration = glm::vec3(ax, ay, az);

		//初始化生命周期
		particle.lifetime = LIFE_TIME;

		//消失速度
		dec = rand() / float(RAND_MAX) * DEC_MAX;
		particle.dec = dec;

		//初始化大小
		particle.scale = rand() / float(RAND_MAX) * (SCALE_MAX - SCALE_MIN) + SCALE_MIN;

	}

	void Update(float dt, glm::vec3 cameraPos) {
		for (Particle& particle : particles) {
			/** 更新位置 */
			position = particle.position;
			velocity = particle.velocity;
			acceleration = particle.acceleration;
			lifetime = particle.lifetime;
			dec = particle.dec;

			position.x += velocity.x;
			position.y -= velocity.y;
			// velocity.y *dt * DT_SCALE

			/** 更新速度 */
			velocity.y += acceleration.y * dt * DT_SCALE;

			/** 更新生存时间 */
			lifetime -= dec * dt * DT_SCALE;

			/** 如果粒子消失或生命结束 */
			if (position.y <= -1 || lifetime <= 0)
			{
				respawnParticle(particle);
				return;


			}

			particle.position = position;
			particle.velocity = velocity;
			particle.acceleration = acceleration;
			particle.lifetime = lifetime;
			particle.dec = dec;

		}
		setGenPos(cameraPos);
	}

	void Draw(Shader& shader, glm::mat4 view, glm::mat4 projection) {
		shader.Use();
		/*projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();*/
		shader.SetMatrix4("projection", projection);
		shader.SetMatrix4("view", view);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, snowTexture);
		snowTexture.Bind();
		for (Particle& particle : particles) {
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, particle.position);
			model = glm::translate(model, particlesGenPos);
			model = glm::rotate(model, glm::radians(particle.rotateAngle.x * particle.rotateSpeed * (float)glfwGetTime()), glm::vec3(1.0, 0.0, 0.0));
			model = glm::rotate(model, glm::radians(particle.rotateAngle.y * particle.rotateSpeed * (float)glfwGetTime()), glm::vec3(0.0, 1.0, 0.0));
			shader.SetMatrix4("model", model);
			//shader.SetVector4f("color", particle.color);
			shader.SetFloat("scale", particle.scale);
			glDrawArrays(GL_TRIANGLES, 0, 6);

		}
		glBindVertexArray(0);
	}

	void setGenPos(glm::vec3 pos) {
		this->particlesGenPos = pos;
	}
};

#endif