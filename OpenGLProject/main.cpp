#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Camera.h"
#include "HeightMap.h"
#include "MeshManager.h"
#include "TextureManager.h"

#include "Entity.h"
#include "ECSManager.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "SizeComponent.h"
//#include "AxisAlignedBoxComponent.h"

#include "GlobalVars.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame




float currentFrame = 0;

fwork::Camera cam;

int main()
{
	srand(time(NULL));

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::string resDir = "Models/";
	//std::string path = resDir + "barrel.obj";


	fwork::Shader shader;
	fwork::TextureManager texManager;
	fwork::MeshManager meshManager;

	const std::vector<float>* barrelVertices;
	const std::vector<float>* deskVertices;

	meshManager.loadMesh(resDir + "barrel.obj");
	meshManager.loadMesh(resDir + "desk.obj");

	barrelVertices = &meshManager.getMeshVertices(resDir + "barrel.obj");
	deskVertices = &meshManager.getMeshVertices(resDir + "desk.obj");

	std::string barrel_t_path = resDir + meshManager.getMeshTexturePath(resDir + "barrel.obj");
	std::string desk_t_path = resDir + meshManager.getMeshTexturePath(resDir + "desk.obj");
	texManager.loadTexture(barrel_t_path);
	texManager.loadTexture(desk_t_path);


	ecs::ECSManager ecsManager;

	std::vector<Entity*> entities;
	
	// Desk
	Entity* desk = new Entity;
	desk->addComponent(ecsManager.newComponent(new ecs::MeshComponent()));
	meshManager.setUpMeshComponent(*desk->getComponent<ecs::MeshComponent>(), resDir + "desk.obj");
	desk->getComponent<ecs::MeshComponent>()->setTexture(texManager.getTexture(desk_t_path));
	desk->addComponent(ecsManager.newComponent(new ecs::TransformComponent()));
	desk->getComponent<ecs::TransformComponent>()->scale(2, 1.5, 2);
	desk->addComponent(ecsManager.newComponent(new ecs::SizeComponent()));
	desk->getComponent<ecs::SizeComponent>()->recalculateSize(*deskVertices, desk->getComponent<ecs::TransformComponent>(), 2);
	fwork::Vec3 deskSize = desk->getComponent<ecs::SizeComponent>()->getSize();

	// Barrels
	for (int i = 0; i < 3; i++)
	{
		Entity* entity = new Entity;

		entity->addComponent(ecsManager.newComponent(new ecs::TransformComponent()));
		entity->addComponent(ecsManager.newComponent(new ecs::MeshComponent()));
		entity->addComponent(ecsManager.newComponent(new ecs::SizeComponent()));

		meshManager.setUpMeshComponent(*entity->getComponent<ecs::MeshComponent>(), resDir + "barrel.obj");
		entity->getComponent<ecs::MeshComponent>()->setTexture(texManager.getTexture(barrel_t_path));

		entity->getComponent<ecs::TransformComponent>()->scale(1, 1 + (rand() % 20)/10.f, 1);

		entity->getComponent<ecs::SizeComponent>()->recalculateSize(*barrelVertices, entity->getComponent<ecs::TransformComponent>(), 2);
		fwork::Vec3 barrelSize = entity->getComponent<ecs::SizeComponent>()->getSize();
		entity->getComponent<ecs::TransformComponent>()->move(-barrelSize.x * (i-1) , deskSize.y/2.f + barrelSize.y/2.f , 0);

		entities.push_back(entity);
	}
	
	entities.push_back(desk);
	
	
	
	//std::cout << s.x << "  " << s.y << "  " << s.z;


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSwapInterval(0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);


		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//map.draw(cam);
		//mesh->draw(shader, cam);

		shader.use();
		cam.applyCamera(shader.ID);
		shader.setInt("tex", 0);
		shader.setBool("textured", true);

		
		for (Entity* entity : entities)
		{
			shader.use();
			shader.setMat4("model_mat4", entity->getComponent<ecs::TransformComponent>()->getTransform());
			entity->getComponent<ecs::MeshComponent>()->draw();


		}

	


		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//map.terminate();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.moveForward(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.moveBackward(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.strafeLeft(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.strafeRight(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cam.moveUp(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cam.moveDown(deltaTime);
}


GLFWwindow* init()
{
	return nullptr;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam.mouseViewOrient(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT_ALT)
		{
			glfwSetInputMode(window, GLFW_CURSOR, (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			cam.setMouseLook((glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? false : true);
			(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ?
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse : ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}
	}

}

