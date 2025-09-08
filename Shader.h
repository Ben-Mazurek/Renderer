//General class for setting up a shader and storing the shaderprogram
#pragma once
#include<iostream>
#include<fstream>
#include<glad/glad.h> //glad is a library that creates a loader for OpenGL and allows us to use it
#include<GLFW/glfw3.h> //GLFW is a library to simplify creating windows and configuring OpenGL

class Shader
{
private:
	std::string readFromFile(std::string path);
public:
	GLuint shaderProgram;
	Shader(std::string vertPath = "default.vert", std::string fragPath = "default.frag");
	~Shader();
	virtual void Update();
};

