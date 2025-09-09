#pragma once
#include<iostream>
#include"RmShader.h"
#include<glm/glm.hpp>

class Controls
{
private:
	glm::vec3 position = { 0, 0, 5 };
	glm::vec3 eulerRot = { 0, 0, 0 };
	GLFWwindow* window;
	RmShader* shader;
	int xRes, yRes;

	float sensitivity = 6;
	float speed = 1;

	bool windowActive = true;


public:
	Controls(GLFWwindow* _window, RmShader* shader);
	void updateMovement(float dt);
};

