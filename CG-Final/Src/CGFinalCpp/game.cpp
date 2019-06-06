#include <stdlib.h>
#include <time.h>

#include <CGFinalHeader/game/game.h>
#include <CGFinalHeader/resourceManager/resource_manager.h>
#include <CGFinalHeader/customObject/border.h>
#include <CGFinalHeader/customObject/plane.h>
#include <CGFinalHeader/modelObject/ModelObject.h>
#include <CGFinalHeader/camera/camera.h>


// Custom Object
Plane *plane;
Border *border;

// Model Object
//ModelObject *nanosuit;
ModelObject *fiatCar;

// lightPos
glm::vec3 lightPos(0.0f, 10.0f, 0.0f);

// position&shift of car
glm::vec3 carPos(0.0f, 0.0f, 0.0f);
glm::vec3 carShift(0.0f, 0.0f, 0.0f);

Game::Game(GLuint width, GLuint height, Camera *camera)
	: State(GameState::GAME_ACTIVE), isBlinn(false), Width(width), Height(height), camera(camera)
{

}

Game::~Game()
{
	delete plane;
	delete border;
	//delete nanosuit;
	delete fiatCar;
}


void Game::Init()
{
	// set rand seed 
	srand((unsigned int)time(NULL));

	// Load shaders
	ResourceManager::LoadShader("../Resources/shaders/shader.vs", "../Resources/shaders/shader.fs", nullptr, "BasicShader");
	ResourceManager::LoadShader("../Resources/shaders/model_loading.vs", "../Resources/shaders/model_loading.fs", nullptr, "BasicModelShader");

	// Load textures
	ResourceManager::LoadTexture("../Resources/textures/block.png", GL_TRUE, "block");
	ResourceManager::LoadTexture("../Resources/textures/block_solid.png", GL_TRUE, "block_solid");
	ResourceManager::LoadTexture("../Resources/textures/container2.png", GL_TRUE, "container2");
	ResourceManager::LoadTexture("../Resources/textures/container2_specular.png", GL_TRUE, "container2_specular");
	ResourceManager::LoadTexture("../Resources/textures/brickwall.jpg", GL_TRUE, "brickwall");
	ResourceManager::LoadTexture("../Resources/textures/wood.png", GL_TRUE, "wood");
	ResourceManager::LoadTexture("../Resources/textures/awesomeface.png", GL_TRUE, "awesomeface");
	ResourceManager::LoadTexture("../Resources/textures/metal.png", GL_TRUE, "metal");
	ResourceManager::LoadTexture("../Resources/textures/window.png", GL_TRUE, "window");
	ResourceManager::LoadTexture("../Resources/textures/snake_skin.jpg", GL_TRUE, "snake_skin");
	ResourceManager::LoadTexture("../Resources/textures/animal_skin_0.jpg", GL_TRUE, "animal_skin_0");

	// Load models
	//ResourceManager::LoadModel("../Resources/objects/nanosuit/nanosuit.obj", "nanosuit");
	ResourceManager::LoadModel("../Resources/objects/fiat/Fiat_127_A_1971.obj", "fiatCar");

	// New Scene Object
	// plane
	plane = new Plane(ResourceManager::GetShader("BasicShader"), ResourceManager::GetTexture("container2_specular"));
	// border
	border = new Border(ResourceManager::GetShader("BasicShader"), ResourceManager::GetTexture("wood"));

	// nanosuit
	//nanosuit = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("nanosuit"));
	// fiatCar
	fiatCar = new ModelObject(ResourceManager::GetShader("BasicModelShader"), ResourceManager::GetModel("fiatCar"));
}

void Game::Update()
{
	carPos += carShift;
	carShift = glm::vec3(0.0f, 0.0f, 0.0f);
}


void Game::ProcessInput(GLFWwindow *window, Camera_Movement direction, glm::vec3 frontOfCar, glm::vec3 upOfCar, float deltaTime)
{
	float velocity = SPEED * deltaTime;
	glm::vec3 rightOfCar = glm::normalize(glm::cross(frontOfCar, upOfCar));
	if (direction == FORWARD)
		carShift = frontOfCar * velocity;
	if (direction == BACKWARD)
		carShift = -frontOfCar * velocity;
	if (direction == LEFT)
		carShift = rightOfCar * velocity;
	if (direction == RIGHT)
		carShift = -rightOfCar * velocity;
}

void Game::Render()
{
	// Configure view and projection
	glm::mat4 view = this->camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(this->camera->Zoom, (GLfloat)this->Width / this->Height, 0.1f, 100.0f);

	// Configure shaders
	// BasicShader
	ResourceManager::GetShader("BasicShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("BasicShader").Use().SetMatrix4("projection", projection);

	ResourceManager::GetShader("BasicShader").Use().SetInteger("texture", 0);
	ResourceManager::GetShader("BasicShader").Use().SetInteger("blinn", this->isBlinn);
	ResourceManager::GetShader("BasicShader").Use().SetVector3f("viewPos", this->camera->Position);
	ResourceManager::GetShader("BasicShader").Use().SetVector3f("lightPos", lightPos);
	// Custom object that use BasicShader
	plane->Draw();
	border->Draw();

	// BasicModelShader
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("model", model);
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
	// Model object that use BasicShader
	//nanosuit->Draw();
	model = glm::mat4(1.0f);
	model = glm::translate(model, carPos);
	//model = glm::translate(model, glm::vec3(1.5f, 0.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ResourceManager::GetShader("BasicModelShader").Use().SetMatrix4("model", model);

	fiatCar->Draw();

}

