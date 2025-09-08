//Object for generating and storing a VAO and anything else specific to an individual object to be drawn
#pragma once
#include<iostream>
#include<fstream>
#include<glad/glad.h> //glad is a library that creates a loader for OpenGL and allows us to use it
#include<GLFW/glfw3.h> //GLFW is a library to simplify creating windows and configuring OpenGL
#include"Shader.h"

class RenderObject
{
private:
	GLuint VBO, VAO, indexBuffer;
	Shader* shader;
	int indexCount = 0;
	void setupVertexArrays(GLfloat vertices[], int vSize, unsigned int indices[], int iSize);
public:
	RenderObject(GLfloat vertices[], int vSize, unsigned int indices[], int iSize, Shader &_shader);
	~RenderObject();
	void draw();

};
