#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb/stb_image.hpp"

#include "shader.h"
#include "vbo.h"
#include "ebo.h"
#include "vao.h"
#include "texture.h"

#include <iostream>
#include "glad/glad.h"
#include "glfw/glfw3.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};


	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "opengl-rendering-engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	VAO vao1;
	vao1.bind();

	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));

	vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.linkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.linkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.id, "scale");

	Texture amogus("textures/amogus.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	amogus.texUnit(shaderProgram, "tex0", 0);

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); //(void)io;
	//ImGui::StyleColorsClassic();
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init("version 330");
	

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.6, 0.776, 0.556, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();*/

		shaderProgram.activate();
		glUniform1f(uniID, 1.5f);
		amogus.bind();
		vao1.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*ImGui::Begin("Test window");
		ImGui::Text("TEST");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	/*ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();*/

	vao1.dispose();
	vbo1.dispose();
	ebo1.dispose();
	amogus.dispose();
	shaderProgram.dispose();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}