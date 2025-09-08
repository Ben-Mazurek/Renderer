#include<iostream>
#include<fstream>
#include<glad/glad.h> //glad is a library that creates a loader for OpenGL and allows us to use it
#include<GLFW/glfw3.h> //GLFW is a library to simplify creating windows and configuring OpenGL
#include "Shader.h"
#include "RenderObject.h"
#include "RmShader.h"
#include "Controls.h"
#include<time.h>



GLfloat vertices[] =
{
	-1.0f, -1.0f, 0.0f,
	1, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

unsigned int indicies[] = { //In order to save on memory we can use the index of a vertex in the vertex array to reuse them
	0, 1, 2,
	1, 3, 2
};

const int resX = 1280;
const int resY = 720;

float deltaTime = 0;

int main() 
{
	glfwInit();

	// We're using OpenGL 3.3, so we tell glfw the major and minor version digits
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//This chooses OpenGL core, which has a lot of useful modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//MSAA
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* window = glfwCreateWindow(resX, resY, "OpenGLTest", NULL, NULL);
	//	A GLFW "context" is an object in which the OpenGL shit is done. We need to add stuff to the context to use it
	glfwMakeContextCurrent(window);

	gladLoadGL();

	//Region of the window we want OpenGL to render in. Two pairs of coordinates for corners.
	glViewport(0, 0, resX, resY);

	RmShader shader1 = RmShader(resX, resY);

	RenderObject triangle = RenderObject(vertices, sizeof(vertices), indicies, sizeof(indicies), shader1);


	//Sets the rgba color to use as default when clearing the color buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clears the back buffer
	glClear(GL_COLOR_BUFFER_BIT);
	//Swaps the currently rendering buffer (front buffer) with the one we just cleared (back buffer)
	glfwSwapBuffers(window);
	

	Controls controls(window, &shader1);

	//Run as long as the window is open
	while (!glfwWindowShouldClose(window))
	{
		time_t startTime = clock();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		controls.updateMovement(deltaTime/CLOCKS_PER_SEC);
		triangle.draw();
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		//Listen to events like window resizing, minimizing, closing.
		glfwPollEvents();
		deltaTime = clock() - startTime;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}