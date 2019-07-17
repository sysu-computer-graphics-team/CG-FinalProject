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
	//����ٶ�
#define MAX_VELOC glm::vec3(0.0,-50.0,0.0)
	//��С�ٶ�
#define MIN_VELOC glm::vec3(0.0,-1.0,0.0)
	//���������������
#define MAX_LAUNCH 1.0f*1000.0f
	//����������С����
#define MIN_LAUNCH 0.3f*1000.0f
	//��ʼ�㾫���С
#define INIT_SIZE 10.0f
#define MAX_SIZE 10.0f
#define MIN_SIZE 3.0f

	const int MAX_PARTICLES = 5000;//�������ӷ���ϵͳ����������
								   //��ʼ��������������
	const int INIT_PARTICLES = 500;
	//��ѩ����
	const glm::vec3 center(0.0f);
	const float areaLength = 150.0f;
	const float fallHeight = 50.0f;

	struct SnowParticle
	{
		float type;
		glm::vec3 position;
		glm::vec3 velocity;
		float lifetimeMills;//����
		float size;//���ӵ㾫���С
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
			//������
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
		void UpdateParticles();//�������ӵ�λ�õ�
		//void InitRandomTexture(unsigned int size);//����1ά�������
		void RenderParticles();
		void GenInitLocation(SnowParticle partciles[], int nums);//���ɳ�ʼ����

		unsigned int mCurVBOIndex, mCurTransformFeedbackIndex;
		GLuint mParticleBuffers[2]; //���ӷ���ϵͳ���������㻺����
		GLuint mParticleArrays[2];
		GLuint mTransformFeedbacks[2];//���ӷ���ϵͳ��Ӧ��TransformFeedback
		//GLuint mRandomTexture;//���һά����
		//CTexture mSparkTexture;//Alpha����
		//CTexture mStartTexture;
		//float mTimer;//���ӷ������Ѿ������ʱ��
		bool mFirst;
		//Shader* mUpdateShader;//�������ӵ�GPUProgram
		//Shader* mRenderShader;//��Ⱦ���ӵ�GPUProgram
	};
}