
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CGFinalHeader/particle/particle.h>
#include <assimp/cimport.h>
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace glm;
using namespace std;

typedef struct Shape
{
	GLuint vao;
	GLuint vbo_position;
	GLuint vbo_normal;
	GLuint vbo_texcoord;
	GLuint ibo;
	int drawCount;
	int materialID;
};

class SnowFall {
	struct {
		GLuint vao;
		GLuint vbo;
		GLuint program;
		GLint um4m;
		GLint um4v;
		GLint um4p;
		GLint tex;

	}snow_program;

	typedef struct _TextureData
	{
		_TextureData(void) :
			width(0),
			height(0),
			data(0)
		{
		}

		int width;
		int height;
		unsigned char* data;
	} TextureData;

	GLuint  snowtex;
	CParticle Snow;
	float x, y, z, vx, vy, vz, ax, ay, az;
	glm::vec3 cameraposition;
	int t1;
	int snow_open = 1;
	Shape* shapes;
	//Material *materials;
	const aiScene* scene;

	void loadModel(string path)
	{
		scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		//materials = (Material*)malloc(sizeof(Material) * scene->mNumMaterials);
		shapes = (Shape*)malloc(sizeof(Shape) * scene->mNumMeshes);
		t1 = scene->mNumMeshes;

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];
			Shape shape;
			glGenVertexArrays(1, &shape.vao);
			glBindVertexArray(shape.vao);

			glGenBuffers(1, &shape.vbo_position);
			glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_position);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			/*glGenBuffers(1, &shape.vbo_texcoord);
			glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_texcoord);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 2 * sizeof(float), 0, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);*/

			glGenBuffers(1, &shape.vbo_normal);
			glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_normal);
			glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), 0, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
			{
				glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_position);
				glBufferSubData(GL_ARRAY_BUFFER, v * sizeof(float) * 3, sizeof(float) * 3, &mesh->mVertices[v][0]);
				//glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_texcoord);
				//glBufferSubData(GL_ARRAY_BUFFER, v * sizeof(float) * 2, sizeof(float) * 2, &mesh->mTextureCoords[0][v][0]);
				glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_normal);
				glBufferSubData(GL_ARRAY_BUFFER, v * sizeof(float) * 3, sizeof(float) * 3, &mesh->mNormals[v][0]);
			}

			glGenBuffers(1, &shape.ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(unsigned int), 0, GL_STATIC_DRAW);
			for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.ibo);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, f * sizeof(unsigned int) * 3, sizeof(unsigned int) * 3, &mesh->mFaces[f].mIndices[0]);
			}

			shape.materialID = mesh->mMaterialIndex;
			shape.drawCount = mesh->mNumFaces * 3;

			shapes[i] = shape;
		}
		aiReleaseImport(scene);
	}

	GLuint CreateTexture(string _strPath, vec2 _v2Size = vec2()) {

		GLuint iTextureID;
		glGenTextures(1, &iTextureID);
		glBindTexture(GL_TEXTURE_2D, iTextureID);
		if (_strPath != "") {
			TextureData tdata = loadPNG(_strPath.c_str());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _v2Size.x, _v2Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//printf("TextureID : %d\n", iTextureID);
		return iTextureID;
	}
	TextureData loadPNG(const char* pngFilepath)
	{
		TextureData texture;
		int components;

		// load the texture with stb image, force RGBA (4 components required)
		stbi_uc* data = stbi_load(pngFilepath, &texture.width, &texture.height, &components, 4);

		// is the image successfully loaded?
		if (data != NULL)
		{
			// copy the raw data
			size_t dataSize = texture.width * texture.height * 4 * sizeof(unsigned char);
			texture.data = new unsigned char[dataSize];
			memcpy(texture.data, data, dataSize);

			// mirror the image vertically to comply with OpenGL convention
			for (size_t i = 0; i < texture.width; ++i)
			{
				for (size_t j = 0; j < texture.height / 2; ++j)
				{
					for (size_t k = 0; k < 4; ++k)
					{
						size_t coord1 = (j * texture.width + i) * 4 + k;
						size_t coord2 = ((texture.height - j - 1) * texture.width + i) * 4 + k;
						std::swap(texture.data[coord1], texture.data[coord2]);
					}
				}
			}

			// release the loaded image
			stbi_image_free(data);
		}
		else
		{
			printf("%s Load PNG fail!\n", pngFilepath);
		}

		return texture;
	}

	//--------- 初始化雪花粒子------------//
	int sky_width = 800;
	bool InitSnow()
	{
		for (int i = 0; i < Snow.GetNumOfParticle(); ++i)
		{
			///初始化坐标
			x = (rand() % sky_width) - (sky_width / 2.0);
			//y = 50 + 0.1f * (rand() % 10) + (cameraposition.y);
			y = (rand() % 90);
			z = (rand() % sky_width) - (sky_width / 2.0);
			Snow.SetPosition(i, x, y, z);
			//printf("(%f, %f, %f)\n", x, y, z);
			///初始化速度
			/*vx = 0.0001 * (rand() % 100);
			vy = (float)(0.0000004 * (rand() % 28000)) + 0.30;
			vz = 0.0001 * (rand() % 100);*/
			// test code
			vx = 10.0f;
			vy = 10.0f;
			vz = 10.0f;
			Snow.SetVelocity(i, vx, vy, vz);

			///初始化加速度
			/*ax = 0;
			ay = 0.000005f;
			az = 0;*/
			// test code
			ax = 10.0f;
			ay = 10.0f;
			az = 10.0f;
			Snow.SetAcceleration(i, ax, ay, az);

		}
		return true;
	}

	//----------------- 更新粒子------------ //
	void UpdateSnow()
	{
		//更新位置 
		x += (vx * 5);
		y -= vy;
		z += (vz * 5);

		// 更新速度 
		vy += ay;

		//如果粒子消失或生命结束 
		if (y <= -3)
		{
			// 初始化位置 
			x = (rand() % sky_width) - (sky_width / 2.0);
			y = (rand() % 60) + 100;
			z = (rand() % sky_width) - (sky_width / 2.0);
			//printf("%f, %f, %f\n", cameraposition.x, cameraposition.y, cameraposition.z);
			//初始化速度 
			vx = (float)(0.0001 * (rand() % 100));
			vy = (float)(0.0000004 * (rand() % 28000)) + 0.30;
			vz = 0.0001 * (rand() % 100);

			// 初始化加速度 
			ax = 0;
			ay = 0.000005f;
			az = 0;

		}
	}
public:
	void DrawParticle(glm::mat4 view, glm::mat4 projection)
	{
		if (true)
		{
			//glUseProgram(snow_program.program);
			Shader shader = ResourceManager::GetShader("snowfallShader");
			shader.Use();
			shader.SetMatrix4("um4p", projection);
			shader.SetMatrix4("um4v", view);

			//glUniformMatrix4fv(snow_program.um4v, 1, GL_FALSE, glm::value_ptr(view));
			//glUniformMatrix4fv(snow_program.um4p, 1, GL_FALSE, glm::value_ptr(projection));
			glBindVertexArray(snow_program.vao);
			glm::mat4 model;
			for (int i = 0; i < Snow.GetNumOfParticle(); ++i)
			{
				// 获得粒子的所有属性 
				Snow.GetAll(i, x, y, z, vx, vy, vz, ax, ay, az);
				model = glm::translate(mat4(), vec3(x, y, z)); //* rotate(mat4(), radians(move), vec3(0.0, 1.0, 0.0));
				//glUniformMatrix4fv(snow_program.um4m, 1, GL_FALSE, value_ptr(model));
				model = glm::scale(model, glm::vec3(10.0f));
				shader.SetMatrix4("um4m", model);
				for (int i = 0; i < t1; ++i)
				{
					glBindVertexArray(shapes[i].vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapes[i].ibo);
					glDrawElements(GL_TRIANGLES, shapes[i].drawCount, GL_UNSIGNED_INT, 0);
				}
				UpdateSnow();
				Snow.SetAll(i, x, y, z, vx, vy, vz, ax, ay, az);
			}
		}
	}

	//GLuint create_program(const char* vs, const char* fs)
	//{
	//	GLuint prog = glCreateProgram();
	//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//	char** vertexShaderSource = loadShaderSource(vs);
	//	char** fragmentShaderSource = loadShaderSource(fs);
	//	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	//	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	//	freeShaderSource(vertexShaderSource);
	//	freeShaderSource(fragmentShaderSource);
	//	glCompileShader(vertexShader);
	//	glCompileShader(fragmentShader);
	//	shaderLog(vertexShader);
	//	shaderLog(fragmentShader);
	//	glAttachShader(prog, vertexShader);
	//	glAttachShader(prog, fragmentShader);
	//	glLinkProgram(prog);
	//	return prog;
	//}
	//void freeShaderSource(char** srcp)
	//{
	//	delete[] srcp[0];
	//	delete[] srcp;
	//}
	//char** loadShaderSource(const char* file)
	//{
	//	FILE* fp = fopen(file, "rb");
	//	fseek(fp, 0, SEEK_END);
	//	long sz = ftell(fp);
	//	fseek(fp, 0, SEEK_SET);
	//	char* src = new char[sz + 1];
	//	fread(src, sizeof(char), sz, fp);
	//	src[sz] = '\0';
	//	char** srcp = new char* [1];
	//	srcp[0] = src;
	//	return srcp;
	//}
	//void shaderLog(GLuint shader)
	//{
	//	GLint isCompiled = 0;
	//	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	//		// The maxLength includes the NULL character
	//		GLchar* errorLog = new GLchar[maxLength];
	//		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

	//		printf("%s\n", errorLog);
	//		delete[] errorLog;
	//	}
	//}

	void InitSnowProgram()
	{
		//snow_program.program = create_program("snow_vertex.glsl", "snow_fragment.glsl");
		snow_program.um4p = glGetUniformLocation(snow_program.program, "um4p");
		snow_program.um4m = glGetUniformLocation(snow_program.program, "um4m");
		snow_program.um4v = glGetUniformLocation(snow_program.program, "um4v");
		snow_program.tex = glGetUniformLocation(snow_program.program, "tex");

		Snow.Create(3000);
		//初始化粒子 
		InitSnow();
		loadModel("../Resources/objects/sphere/Sphere.obj");
		shapes->vbo_texcoord = CreateTexture("../Resources/textures/snowball.png");
		ResourceManager::LoadShader("../Resources/shaders/snow_vertex.glsl", "../Resources/shaders/snow_fragment.glsl", nullptr, "snowfallShader");
	}
	void setCamera(vec3 camera)
	{
		cameraposition = camera;
	}
	void OpenSnow()
	{
		snow_open = 1;
	}
	void CloseSnow()
	{
		snow_open = 0;
	}
};