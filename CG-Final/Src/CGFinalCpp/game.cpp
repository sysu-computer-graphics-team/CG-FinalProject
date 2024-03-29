#include <stdlib.h>
#include <time.h>

#include <CGFinalHeader/game/game.h>
#include <CGFinalHeader/resourceManager/resource_manager.h>
#include <CGFinalHeader/modelObject/ModelObject.h>
#include <CGFinalHeader/camera/camera.h>
#include <CGFinalHeader/skybox/skybox.h>
#include <CGFinalHeader/scene/scene.h>
#include <CGFinalHeader/customObject/lamp.h>
#include <CGFinalHeader/particle/particle.h>


Lamp *lamp;

// Model Object
Scene* scene;

//ModelObject *nanosuit;
ModelObject *fiatCar;
ModelObject *oldHouse;
ModelObject *temple;
ModelObject *tree;
bool renderTempleFlag = true;

// Skybox
Skybox* skybox;

// Particle
ParticleGenerator* particles;

// lightPos
glm::vec3 lightPos(0.0001f, 10.0f, 0.0f);

// position&shift of car
glm::vec3 carShift(0.0f, 0.0f, 0.0f);

const bool renderSkybox = true;
//用来估算车子的大小，若要精细一点的话，因为carPos好像是在车头，所以可能需要四个变量：AddCarX/Y & MinusCarX/Y
const unsigned int carSizeX = 1, carSizeY = 1;

float timeCounter = -1.5708f;
float housetime[4] = { -1.5708f ,-1.5708f ,-1.5708f ,-1.5708f };

float brokehousez[4] = { 3,13,23,33 };
bool brokehouseflag[4] = { false,false,false,false };
bool renderhouseflag[4] = { true,true,true,true };
// shadow size
const unsigned int SHADOW_WIDTH = 1280, SHADOW_HEIGHT = 1280;

Game::Game(GLuint width, GLuint height, Camera *camera)
	: State(GameState::GAME_ACTIVE), isBlinn(false), Width(width), Height(height), camera(camera)
{

}

Game::~Game()
{
	delete lamp;
	//delete nanosuit;
	delete scene;
	delete fiatCar;
	delete skybox;
	delete oldHouse;
	delete tree;
	delete temple;
}


void Game::Init()
{
	// set rand seed 
	srand((unsigned int)time(NULL));

	// Load shaders
	ResourceManager::LoadShader("../Resources/shaders/shader.vs", "../Resources/shaders/shader.fs", nullptr, "BasicShader");
	ResourceManager::LoadShader("../Resources/shaders/model_loading.vs", "../Resources/shaders/model_loading.fs", nullptr, "BasicModelShader");
	ResourceManager::LoadShader("../Resources/shaders/skyShader.vs", "../Resources/shaders/skyShader.fs", nullptr, "skyShader");
	ResourceManager::LoadShader("../Resources/shaders/simpleDepthShader.vs", "../Resources/shaders/simpleDepthShader.fs", nullptr, "DepthShader");
	ResourceManager::LoadShader("../Resources/shaders/shadow_mapping.vs", "../Resources/shaders/shadow_mapping.fs", nullptr, "ShadowShader");
	ResourceManager::LoadShader("../Resources/shaders/lamp.vs", "../Resources/shaders/lamp.fs", nullptr, "LampShader");
	ResourceManager::LoadShader("../Resources/shaders/textShader.vs", "../Resources/shaders/textShader.fs", nullptr, "textShader");
	ResourceManager::LoadShader("../Resources/shaders/explode.vs", "../Resources/shaders/explode.fs", "../Resources/shaders/explode.gs", "explodeShader");
	ResourceManager::LoadShader("../Resources/shaders/particleShader.vs", "../Resources/shaders/particleShader.fs", nullptr, "particleShader");
	
	// Load textures
	 ResourceManager::LoadTexture("../Resources/textures/block.png", GL_TRUE, "block");
	// ResourceManager::LoadTexture("../Resources/textures/block_solid.png", GL_TRUE, "block_solid");
	// ResourceManager::LoadTexture("../Resources/textures/container2.png", GL_TRUE, "container2");
	// ResourceManager::LoadTexture("../Resources/textures/container2_specular.png", GL_TRUE, "container2_specular");
	// ResourceManager::LoadTexture("../Resources/textures/brickwall.jpg", GL_TRUE, "brickwall");
	ResourceManager::LoadTexture("../Resources/textures/wood.png", GL_TRUE, "wood");
	ResourceManager::LoadTexture("../Resources/textures/window.png", GL_TRUE, "window");
	// ResourceManager::LoadTexture("../Resources/textures/awesomeface.png", GL_TRUE, "awesomeface");
	// ResourceManager::LoadTexture("../Resources/textures/metal.png", GL_TRUE, "metal");
	// ResourceManager::LoadTexture("../Resources/textures/window.png", GL_TRUE, "window");
	// ResourceManager::LoadTexture("../Resources/textures/snake_skin.jpg", GL_TRUE, "snake_skin");
	// ResourceManager::LoadTexture("../Resources/textures/animal_skin_0.jpg", GL_TRUE, "animal_skin_0");
	ResourceManager::LoadTexture("../Resources/textures/particle.png", GL_TRUE, "particle");

	/* skybox */
	vector<std::string> faces{
		"../Resources/textures/origin/right.jpg",
		"../Resources/textures/origin/left.jpg",
		"../Resources/textures/origin/top.jpg",
		"../Resources/textures/origin/bottom.jpg",
		"../Resources/textures/origin/front.jpg",
		"../Resources/textures/origin/back.jpg"
	};
	unsigned int cubemapTexture = ResourceManager::LoadCubemap(faces);

	// Load models
	// ResourceManager::LoadModel("../Resources/objects/nanosuit/nanosuit.obj", "nanosuit");
	ResourceManager::LoadModel("../Resources/objects/fiat/Fiat_127_A_1971.obj", "fiatCar");
	ResourceManager::LoadModel("../Resources/objects/Big_Old_House/Big_Old_House.obj", "oldHouse");
	ResourceManager::LoadModel("../Resources/objects/building/Model/Japanese_Temple.obj", "temple");
	ResourceManager::LoadModel("../Resources/objects/tree/tree.obj", "tree");
	// New Scene Object
	scene = new Scene();
	// lamp
	lamp = new Lamp(ResourceManager::GetShader("LampShader"));
	lamp->lightPos = &lightPos;
	// skybox
	skybox = new Skybox(ResourceManager::GetShader("skyShader"), cubemapTexture);

	// nanosuit
	// nanosuit = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("nanosuit"));
	// fiatCar
	fiatCar = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("fiatCar"));
	oldHouse = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("oldHouse"));
	temple = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("temple"));
	tree = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("tree"));
	// shadow mapping configure
	glGenFramebuffers(1, &this->depthMapFBO);
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mytext.init();

	particles = new ParticleGenerator(ResourceManager::GetShader("particleShader"), ResourceManager::GetTexture("particle"), 500);
}

void Game::Update(GLfloat dt)
{
	glm::vec3 position = carPos + carShift;
	float size = 39;
	float road = 6;
	float border = 1;
	if (position.x > border && position.z > border && position.x < (size - border) && position.z < (size - border)) {
		if (position.x < road || position.z<road || position.x >(size - road) || position.z >(size - road))
			carPos = position;
	}

	particles->Update(dt, 2, carShift, carPos, glm::vec2(100.0f, 100.0f));
	carShift = glm::vec3(0.0f, 0.0f, 0.0f);

}


void Game::ProcessInput(GLFWwindow *window, Camera_Movement direction, glm::vec3 frontOfCar, glm::vec3 upOfCar, float deltaTime)
{
	float velocity = SPEED * deltaTime*0.10 * abs(carSpeed);
	glm::vec3 front;
	front.x = -cos(glm::radians(Yaw));
	front.y = 0;
	front.z = sin(glm::radians(Yaw));
	carfront = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	glm::vec3 rightOfCar = glm::normalize(glm::cross(carfront, upOfCar));
	if (direction == FORWARD) {
		carShift += carfront * velocity;
	}
	if (direction == LEFT_FORWARD) {
		Yaw += 2;
	}
	if (direction == RIGHT_FORWARD) {
		Yaw -= 1;
		float v = velocity * 2;
		carShift += carfront * v;
	}
	if (direction == LEFT_BACKWARD) {
		Yaw -= 2;
	}
	if (direction == RIGHT_BACKWARD) {
		Yaw += 1;
		float v = velocity * 2;
		carShift += -carfront * v;
	}
	if (direction == BACKWARD) {
		carShift += -carfront * velocity;
	}
}

void Game::Render()
{
	// 1. render depth of scene to texture (from light's perspective)

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	// default use ortho projection
	lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 30.0f);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	// reander scane from light's point of view
	ResourceManager::GetShader("DepthShader").Use().SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	// render scene
	// for custom object, model matrix already configure in Draw() function
	scene->Draw("DepthShader", nullptr);
	// for model object, model matrix do not configure in Draw() function, we need to configure model matrix!

	//render car shadow
	fiatCar->shader = ResourceManager::GetShader("DepthShader");
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, carPos);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	// here should pass a bool value to Mesh Draw, if is depthshader, then do not configure texture
	fiatCar->Draw(depthMap);

	//render old house shadow
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	for (int i = 0; i < 4; i++) {
		oldHouse->Draw(depthMap);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	}

	//render tree shadow
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 80.0f));
	ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	for (int i = 0; i < 8; i++) {
		tree->Draw(depthMap);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	}

	//render temple shadow
	if (renderTempleFlag) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(19.0f, 0.0f, 19.0f));
		ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
		temple->Draw(depthMap);
	}
	//render broke house shadow
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(70.0f, 0.0f, 10.0f));
	ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	for (int i = 0; i < 4; i++) {
		if (renderhouseflag[i]) {
			oldHouse->Draw(depthMap);
		}		
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		ResourceManager::GetShader("DepthShader").Use().SetMatrix4("model", model);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// -------------------------------------------------------------------------------------------------------------

	// 2. render scene as normal using the generated depth/shadow map
	glViewport(0, 0, this->Width, this->Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Configure view and projection
	glm::mat4 view = this->camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(this->camera->Zoom), (GLfloat)this->Width / this->Height, 0.1f, 100.0f);

	// Configure shaders
	// LampShader
	ResourceManager::GetShader("LampShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("LampShader").Use().SetMatrix4("projection", projection);
	lamp->Draw();
	// ShadowShader
	ResourceManager::GetShader("ShadowShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("ShadowShader").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("ShadowShader").Use().SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);
	ResourceManager::GetShader("ShadowShader").Use().SetInteger("diffuseTexture", 0);
	ResourceManager::GetShader("ShadowShader").Use().SetInteger("shadowMap", 1);
	// ResourceManager::GetShader("ShadowShader").Use().SetInteger("blinn", this->isBlinn);
	ResourceManager::GetShader("ShadowShader").Use().SetVector3f("viewPos", this->camera->Position);
	ResourceManager::GetShader("ShadowShader").Use().SetVector3f("lightPos", lightPos);
	// Custom object that use ShadowShader
	scene->Draw("ShadowShader", &this->depthMap);

	// BasicModelShader
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("view", this->camera->GetViewMatrix());
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("projection", projection);

	ResourceManager::GetShader("BasicModelShader").Use().SetVector3f("viewPos", this->camera->Position);
	ResourceManager::GetShader("BasicModelShader").Use().SetVector3f("pointlight.position", lightPos);
	ResourceManager::GetShader("BasicModelShader").Use().SetVector3f("pointlight.ambient", 0.2f, 0.2f, 0.2f);
	ResourceManager::GetShader("BasicModelShader").Use().SetVector3f("pointlight.diffuse", 0.8f, 0.8f, 0.8f);
	ResourceManager::GetShader("BasicModelShader").Use().SetVector3f("pointlight.specular", 1.0f, 1.0f, 1.0f);
	ResourceManager::GetShader("BasicModelShader").Use().SetFloat("pointlight.constant", 1.0f);
	ResourceManager::GetShader("BasicModelShader").Use().SetFloat("pointlight.linear", 0.022f);
	ResourceManager::GetShader("BasicModelShader").Use().SetFloat("pointlight.quadratic", 0.0019f);
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);
	ResourceManager::GetShader("BasicModelShader").Use().SetInteger("shadowMap", 1);
	// Model object that use BasicShader
	// nanosuit->Draw();

	model = glm::mat4(1.0f);
	model = glm::translate(model, carPos);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("model", model);
	fiatCar->shader = ResourceManager::GetShader("BasicModelShader");
	fiatCar->Draw(depthMap);

	//render house
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("model", model);
	oldHouse->shader = ResourceManager::GetShader("BasicModelShader");
	for (int i = 0; i < 4; i++) {
		oldHouse->Draw(depthMap);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		oldHouse->shader.Use().SetMatrix4("model", model);
	}
	//render trees
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 76.0f));
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("model", model);
	tree->shader = ResourceManager::GetShader("BasicModelShader");
	for (int i = 0; i < 8; i++) {
		tree->Draw(depthMap);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		tree->shader.Use().SetMatrix4("model", model);
	}

	//render temple
	temple->shader = ResourceManager::GetShader("BasicModelShader");
	if (IsConflict())
	{
		ResourceManager::GetShader("explodeShader").Use().SetMatrix4("model", model);
		ResourceManager::GetShader("explodeShader").Use().SetMatrix4("view", view);
		ResourceManager::GetShader("explodeShader").Use().SetMatrix4("projection", projection);
		ResourceManager::GetShader("explodeShader").Use().SetFloat("time", timeCounter);
		timeCounter = (timeCounter + 0.015f) <= 1.5f ? (timeCounter + 0.015f) : 1.5f;
		if (timeCounter >= 1.5f)
			renderTempleFlag = false;
		temple->shader = ResourceManager::GetShader("explodeShader");
	}
	if (renderTempleFlag) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(19.0f, 0.0f, 19.0f));
		temple->shader.Use().SetMatrix4("model", model);
		temple->Draw(depthMap);
	}
	//render broke house
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(70.0f, 0.0f, 10.0f));
	for (int i = 0; i < 4; i++) {
		if (renderhouseflag[i]) {
			IsConflict(i);
			if (brokehouseflag[i]) {
				ResourceManager::GetShader("explodeShader").Use().SetMatrix4("model", model);
				ResourceManager::GetShader("explodeShader").Use().SetMatrix4("view", view);
				ResourceManager::GetShader("explodeShader").Use().SetMatrix4("projection", projection);
				ResourceManager::GetShader("explodeShader").Use().SetFloat("time", housetime[i]);
				housetime[i] = (housetime[i] + 0.015f) <= 1.5f ? (housetime[i] + 0.015f) : 1.5f;
				if (housetime[i] >= 1.5f)
					renderhouseflag[i] = false;
				oldHouse->shader = ResourceManager::GetShader("explodeShader");
			}
			else
			{
				oldHouse->shader = ResourceManager::GetShader("BasicModelShader");
			}
			oldHouse->shader.Use().SetMatrix4("model", model);
			oldHouse->Draw(depthMap);			
		}		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
	}


	// Skybox Shader
	ResourceManager::GetShader("skyShader").Use().SetMatrix4("view",
		glm::mat4(glm::mat3(this->camera->GetViewMatrix())));
	ResourceManager::GetShader("skyShader").Use().SetMatrix4("projection", projection);
	if (renderSkybox) {
		skybox->Draw();
	}

	// particle test
	ResourceManager::GetShader("particleShader").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particleShader").SetMatrix4("projection", projection);
	
	// Compile and setup the textshader
	projection = glm::ortho(0.0f, static_cast<GLfloat>(Width), 0.0f, static_cast<GLfloat>(Height));
	ResourceManager::GetShader("textShader").Use().SetMatrix4("projection", projection);
	mytext.RenderText(ResourceManager::GetShader("textShader"), str, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	mytext.RenderText(ResourceManager::GetShader("textShader"), "press C to change camera", 425.0f, 25.0f, 0.5f, glm::vec3(0.0, 0.0f, 0.8f));
}

bool Game::IsConflict() {
	//std::cout << carPos.x << " " << carPos.y << " " << carPos.z << endl;
	bool collisionX = abs(carPos.x + carSizeX -1.5) <= 1.5 || abs(carPos.x - carSizeX-1.5) <= 1.5;
	bool collisionY = abs(carPos.z + carSizeY-20) <= 1.5 || abs(carPos.z - carSizeY-20) <= 1.5;
	//cout << collisionX << " " << collisionY << endl;
	// Collision only if on both axes
	//return collisionX && collisionY;
	return false;
}
void Game::IsConflict(int i) {
	//std::cout << carPos.x << " " << carPos.y << " " << carPos.z << endl;
	bool collisionX = abs(carPos.x + carSizeX - 35.4) <= 1.5 || abs(carPos.x - carSizeX - 35.4) <= 1.5;
	bool collisionY = abs(carPos.z + carSizeY - brokehousez[i]) <= 1.5 || abs(carPos.z - carSizeY - brokehousez[i]) <= 1.5;
	//cout << collisionX << " " << collisionY << endl;
	// Collision only if on both axes
	if (collisionX && collisionY) {
		brokehouseflag[i] = true;
	}
}

glm::vec3 Game::getFrontOfCar() {
	return carfront;
}


