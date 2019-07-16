#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <CGFinalHeader/camera/camera.h>
#include <CGFinalHeader/game/game.h>
#include <CGFinalHeader/resourceManager/resource_manager.h>
#include <ft2build.h>
#include FT_FREETYPE_H 
// glfw callback function
static void glfw_error_callback(GLint error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// also process input (different from mouse_callback and key_callback)
void processInput(GLFWwindow* window);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The Height of the screen
const GLuint SCREEN_HEIGHT = 600;
// Mouse capture flag
bool mouseCaptured = true;

// camera
// Camera camera(glm::vec3(-0.2f, 15.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90, -50);
Camera camera(glm::vec3(1.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90, -60);
GLfloat lastX = (float)SCREEN_WIDTH / 2.0;
GLfloat lastY = (float)SCREEN_HEIGHT / 2.0;
bool firstMouse = true;

// front&up of car
glm::vec3 frontOfCar = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 upOfCar = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Game SimpleScene(SCREEN_WIDTH, SCREEN_HEIGHT, &camera);

int main(int argc, char* argv[]) {
    // set glfw error callback function before initialization
    // ------------------------------------------------------
    glfwSetErrorCallback(glfw_error_callback);

    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation and configuration
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Program", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);

    // Initialize game
    SimpleScene.Init();

    // Start Game within Menu State
    SimpleScene.State = GameState::GAME_ACTIVE;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        processInput(window);

        // Process Game input
        //SimpleScene.ProcessInput(window, deltaTime);

        // Update Game state
        SimpleScene.Update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render Scene
        SimpleScene.Render();

        // std::cout << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << "\t" << camera.Yaw << "," << camera.Pitch << std::endl;

        glfwSwapBuffers(window);
    }

    // Delete all resources as loaded using the resource maneger
    ResourceManager::Clear();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        if (!mouseCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCaptured = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCaptured = false;
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        SimpleScene.isBlinn = !SimpleScene.isBlinn;
        std::cout << "Lighting: " << ((SimpleScene.isBlinn) ? "Blinn" : "Phong") << std::endl;
    }
}

void processInput(GLFWwindow* window) {
	frontOfCar = SimpleScene.getFrontOfCar();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //camera.ProcessKeyboard(FORWARD, frontOfCar, upOfCar, deltaTime);	
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.ProcessKeyboard(LEFT_FORWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.ProcessInput(window, LEFT_FORWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 30";

		}
		else  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.ProcessKeyboard(RIGHT_FORWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.ProcessInput(window, RIGHT_FORWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 30";
		}
		else {
			SimpleScene.ProcessInput(window, FORWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 50";
		}
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //camera.ProcessKeyboard(BACKWARD, frontOfCar, upOfCar, deltaTime);
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.ProcessKeyboard(LEFT_BACKWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.ProcessInput(window, LEFT_BACKWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 30";
		}
		else  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.ProcessKeyboard(RIGHT_BACKWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.ProcessInput(window, RIGHT_BACKWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 30";
		}
		else {
			SimpleScene.ProcessInput(window, BACKWARD, frontOfCar, upOfCar, deltaTime);
			SimpleScene.str = "speed : 50";
		}
    }
	else SimpleScene.str = "speed : 0";
	glm::vec3 rightOfCar = glm::normalize(glm::cross(frontOfCar, upOfCar));
	glm::vec3 Postion = SimpleScene.carPos;
	glm::vec3 backShift = glm::vec3(-5*frontOfCar.x, -5*frontOfCar.y, -5*frontOfCar.z);
	glm::vec3 rightShift = glm::vec3(1.2*rightOfCar.x, 1.2*rightOfCar.y, 1.2*rightOfCar.z);;
	glm::vec3 cameraShift = backShift + rightShift;
	glm::vec3 cameraHeight = glm::vec3(0, 3, 0);
	camera.setPostion(Postion+cameraHeight+cameraShift);
   
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (mouseCaptured)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (mouseCaptured)
        camera.ProcessMouseScroll(yoffset);
}