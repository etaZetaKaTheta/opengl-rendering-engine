#include "mesh.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "time.h"

int screenWidth = 1280;
int screenHeight = 720;

void reactToFrameResize(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	screenWidth = newWidth;
	screenHeight = newHeight;
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Vertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
		Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Vertex lightVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "opengl-rendering-engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, reactToFrameResize);
	glfwSetWindowAspectRatio(window, 16, 9);

	gladLoadGL();

	glViewport(0, 0, screenWidth, screenHeight);

	Texture textures[]
	{
		Texture("textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh plane(verts, ind, tex);

	Shader lightShader("shaders/light.vert", "shaders/light.frag");
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModelMat = glm::mat4(1.0f);
	lightModelMat = glm::translate(lightModelMat, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModelMat = glm::mat4(1.0f);
	pyramidModelMat = glm::translate(pyramidModelMat, pyramidPos);

	lightShader.activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "modelMat"), 1, GL_FALSE, glm::value_ptr(lightModelMat));
	glUniform4f(glGetUniformLocation(lightShader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "modelMat"), 1, GL_FALSE, glm::value_ptr(pyramidModelMat));
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	float rotation = 0.5f;
	double prevTime = glfwGetTime();

	

	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.388f, 0.278f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Time::calculateDeltaTime();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		camera.updateMatrix(60.0f, 0.001f, 1000.0f, screenWidth, screenHeight);
		camera.inputs(window);
		
		plane.draw(shaderProgram, camera);
		light.draw(lightShader, camera);

		ImGui::SetNextWindowSize(ImVec2(350, 100));
		ImGui::Begin("Test window");
		ImGui::Text("CamOrientation: %f, %f, %f", camera.orientation.x, camera.orientation.y, camera.orientation.z);
		ImGui::Text("CamPosition: %f, %f, %f", camera.position.x, camera.position.y, camera.position.z);
		ImGui::SliderInt("CamMatTest", &screenWidth, 1, 1920);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	shaderProgram.dispose();
	lightShader.dispose();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}