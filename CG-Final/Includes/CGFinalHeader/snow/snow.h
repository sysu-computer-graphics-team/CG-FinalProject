#pragma once

#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <glm/glm.hpp>
#include <CGFinalHeader/shader/shader.h>
#include <CGFinalHeader/camera/camera.h>
//#include "texture.h"

namespace Snow {

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
	//最大速度
#define MAX_VELOC glm::vec3(0.0,-50.0,0.0)
	//最小速度
#define MIN_VELOC glm::vec3(0.0,-1.0,0.0)
	//发射粒子最大周期
#define MAX_LAUNCH 1.0f*1000.0f
	//发射粒子最小周期
#define MIN_LAUNCH 0.3f*1000.0f
	//初始点精灵大小
#define INIT_SIZE 10.0f
#define MAX_SIZE 10.0f
#define MIN_SIZE 3.0f

	const int MAX_PARTICLES = 5000;//定义粒子发射系统最大的粒子数
								   //初始发射器例子数量
	const int INIT_PARTICLES = 500;
	//落雪中心
	const glm::vec3 center(0.0f);
	const float areaLength = 150.0f;
	const float fallHeight = 50.0f;

	struct SnowParticle
	{
		float type;
		glm::vec3 position;
		glm::vec3 velocity;
		float lifetimeMills;//年龄
		float size;//粒子点精灵大小
	};



	class Snow
	{
	public:
		Snow() {
			mCurVBOIndex = 0;
			mCurTransformFeedbackIndex = 1;
			mFirst = true;
		}
		~Snow() {}
		//void Render(Shader* mUpdateShader, Shader* mRenderShader, float frametimeMills, glm::mat4& worldMatrix, glm::mat4 viewMatrix, glm::mat4& projectMatrix);
		bool Init(Shader* mUpdateShader)
		{
			SnowParticle particles[MAX_PARTICLES];
			memset(particles, 0, sizeof(particles));
			GenInitLocation(particles, INIT_PARTICLES);
			glGenTransformFeedbacks(2, mTransformFeedbacks);
			glGenBuffers(2, mParticleBuffers);
			glGenVertexArrays(2, mParticleArrays);
			for (int i = 0; i < 2; i++)
			{
				glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbacks[i]);
				glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffers[i]);
				glBindVertexArray(mParticleArrays[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffers[i]);
				//glBindBuffer(GL_ARRAY_BUFFER,0);
			}
			//glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,0);
			glBindVertexArray(0);
			//绑定纹理
			mUpdateShader->use();
			mUpdateShader->setInt("gRandomTexture", 0);
			mUpdateShader->setFloat("MAX_SIZE", MAX_SIZE);
			mUpdateShader->setFloat("MIN_SIZE", MIN_SIZE);
			mUpdateShader->setFloat("MAX_LAUNCH", MAX_LAUNCH);
			mUpdateShader->setFloat("MIN_LAUNCH", MIN_LAUNCH);
			glUseProgram(0);


			return true;
		}
		void Update();
		void Render();

	private:
		void UpdateParticles();//更新粒子的位置等
		//void InitRandomTexture(unsigned int size);//生成1维随机纹理
		void RenderParticles();
		void GenInitLocation(SnowParticle partciles[], int nums);//生成初始粒子

		unsigned int mCurVBOIndex, mCurTransformFeedbackIndex;
		GLuint mParticleBuffers[2]; //粒子发射系统的两个顶点缓存区
		GLuint mParticleArrays[2];
		GLuint mTransformFeedbacks[2];//粒子发射系统对应的TransformFeedback
		//GLuint mRandomTexture;//随机一维纹理
		//CTexture mSparkTexture;//Alpha纹理
		//CTexture mStartTexture;
		//float mTimer;//粒子发射器已经发射的时间
		bool mFirst;
		//Shader* mUpdateShader;//更新粒子的GPUProgram
		//Shader* mRenderShader;//渲染粒子的GPUProgram
	};
}