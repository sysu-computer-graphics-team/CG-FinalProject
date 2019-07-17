
#ifndef __opengl__Particle__
#define __opengl__Particle__
#ifdef _MSC_VER
#include <direct.h>
#include <glad/glad.h>
#include <iostream>
#else
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <unistd.h>
#endif

// 粒子结构 //
struct myParticle
{
	float x, y, z;            //< 粒子的位置 //
	float vx, vy, vz;         //< 粒子的速度(x,y,z方向) //
	float ax, ay, az;         //< 粒子在x，y，z上的加速度 //

};

// 粒子类 //
class CParticle
{
private:
	myParticle*   data;               //< 粒子指针 //
	int         numparticle;        //< 粒子数目 //

public:
	CParticle();          //< 构造函数 //
	~CParticle();         //< 析构函数 //

	// 创建粒子数组 //
	int Create(long num);

	// 设置和获取速度属性 //
	int SetVelocity(GLfloat vx, GLfloat vy, GLfloat vz);
	int SetVelocity(GLint index, GLfloat vx, GLfloat vy, GLfloat vz);
	int GetVelocity(GLint index, GLfloat &vx, GLfloat &vy, GLfloat &vz);

	// 设置和获取位置属性 //
	int SetPosition(GLfloat x, GLfloat y, GLfloat z);
	int SetPosition(GLint index, GLfloat x, GLfloat y, GLfloat z);
	int GetPosition(GLint index, GLfloat &x, GLfloat &y, GLfloat &z);

	// 设置和获取加速度属性 //
	int SetAcceleration(GLfloat ax, GLfloat ay, GLfloat az);
	int SetAcceleration(GLint index, GLfloat ax, GLfloat ay, GLfloat az);
	int GetAcceletation(GLint index, GLfloat &ax, GLfloat &ay, GLfloat &az);

	// 获取粒子数组地址 //
	myParticle *GetParticle() { return data; }

	// 获得粒子的数目 //
	int GetNumOfParticle() { return numparticle; }

	// 获得粒子所有的属性 
	int GetAll(int index,                               // 索引 
		GLfloat &x, GLfloat &y, GLfloat &z,        // 位置 
		GLfloat &vx, GLfloat &vy, GLfloat &vz, // 速度 
		GLfloat &ax, GLfloat &ay, GLfloat &az //  加速度 
	);

	// 设置粒子的所有属性 
	int SetAll(int index,                           // 索引 
		GLfloat x, GLfloat y, GLfloat z,       // 位置 
		GLfloat vx, GLfloat vy, GLfloat vz,    // 速度 
		GLfloat ax, GLfloat ay, GLfloat az   // 加速度 
	);
};

#endif // defined(__opengl__Particle__) //

