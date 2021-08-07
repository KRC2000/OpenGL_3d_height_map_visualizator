#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Camera.h"
#include "HeightMap.h"


#include "GlobalVars.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);




float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame




float currentFrame = 0;

Camera cam;

int main()
{
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


	//HeightMap map("map.tga", "3.3.heightmap.vert", "3.3.heightmap.frag");
	HeightMap map("map.tga", "Shaders");



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


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSwapInterval(1);

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

		map.draw(cam);


		 // render your GUI
		ImGui::Begin("Settings");
		ImGui::Text("'Left-Alt' to switch cursor mode");
		ImGui::BulletText(("X: " + std::to_string(cam.cameraPos.x)).c_str());
		ImGui::BulletText(("Y: " + std::to_string(cam.cameraPos.y)).c_str());
		ImGui::BulletText(("Z: " + std::to_string(cam.cameraPos.z)).c_str());
		ImGui::SliderFloat("Height multiplier", &map.heightMultiplier, 0.00001f, 20.0f, "%.6f");
		ImGui::Checkbox("Wireframe mode", &map.wireframe);
		ImGui::Checkbox("Textured", &map.textured);
		ImGui::Checkbox("Normals", &map.normals);
		if (map.normals) ImGui::SliderFloat("Normal length", &map.normalLength, 0.01, 3);
		ImGui::Checkbox("Light", &map.light);

		ImGui::Text("'Left-Alt' for free camera");
		//float col1[3] = { 1.0f, 0.0f, 0.2f };
		ImGui::ColorEdit3("color 1", &map.targetColor[0]);

		ImGui::ShowDemoWindow();
		ImGui::End();

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
	map.terminate();

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
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		cam.setMouseLook((glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? false : true);
		(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? 
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse : ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}

}

// Loads TGA image data, creates array with only green channel and returns it.
// Param-s "with" and "height" will be filled with picture size
unsigned char* loadHeightMap(std::string path, int& width, int& height)
{
	int nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
	if (data)
	{
		// Creating separate data arr with only Green channel
		int g_dataSize = width * height;
		unsigned char* g_data = new  unsigned char[g_dataSize];

		unsigned int g_dataIterator = 0;
		for (int i = 1; i < g_dataSize; i+=3)
		{
			g_data[g_dataIterator] = data[i];
			g_dataIterator++;
		}
		return g_data;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

float* genHeightMapPlane(unsigned int* &drawIndices, unsigned int map_width, unsigned int map_height, int freq, int& returnArr_size, int& indicesAmount)
{
	int vertWith = (freq != 0) ? freq + 1 : map_width;
	int vertHeight = (freq != 0) ? freq + 1 : map_height;
	int vertAmount = vertWith * vertHeight;


	// Creating array of needed size
	returnArr_size = vertAmount * 3;
	float* vertices = vertices = new float[vertAmount*3];

	float widthStep = 2.f / (vertWith-1);
	float heightStep = 2.f / (vertHeight-1);

	// Filling array with vertices positions, not ready to draw because points aren't in triangle sequence
	int vert_i = 0;
	for (int z = 0; z < vertHeight; z++)
	{
		for (int x = 0; x < vertWith; x++)
		{
			vertices[vert_i] = -1.f + x * widthStep;
			vertices[vert_i + 1] = 0.f;
			vertices[vert_i + 2] = 1.f + z * -heightStep;

			//std::cout << vertices[vert_i] << " " << vertices[vert_i +1] << " " << vertices[vert_i +2] << std::endl;

			vert_i += 3;
		}
	}
	//std::cout << std::endl;
	// Indices for upper-left triangles
	//  *---*
	//  |  /
	//  | /
	//  |/
	//  *
	indicesAmount = freq * freq * 6;
	drawIndices = new unsigned int[indicesAmount];
	int ind_i = 0;
	for (int y = 0; y < vertHeight - 1; y++)
	{
		for (int x = 0; x < vertWith - 1; x++)
		{
			drawIndices[ind_i] = y * vertWith + x;
			drawIndices[ind_i + 1] = y * vertWith + x + 1;
			drawIndices[ind_i + 2] = y * vertWith + x + vertWith;

			//std::cout << drawIndices[ind_i] << " " << drawIndices[ind_i+1] << " " << drawIndices[ind_i+2] << std::endl;
			ind_i += 3;
		}
	}
	
	//std::cout << std::endl;

	// Indices for down-right triangles
	//      *
	//     /|
	//    / |
	//   /  |
	//  *---*
	for (int y = 0; y < vertHeight - 1; y++)
	{
		for (int x = 1; x < vertWith; x++)
		{
			drawIndices[ind_i] = y * vertWith + x;
			drawIndices[ind_i + 1] = y * vertWith + x + vertWith;
			drawIndices[ind_i + 2] = y * vertWith + x + vertWith - 1;
			//std::cout << drawIndices[ind_i] << " " << drawIndices[ind_i + 1] << " " << drawIndices[ind_i + 2] << std::endl;
			ind_i += 3;
		}
	}

	return vertices;
}
