#include "Controls.h"

Controls::Controls(GLFWwindow* _window, RmShader* _shader) {
	window = _window;
	shader = _shader;
	glfwGetWindowSize(window, &xRes, &yRes);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Controls::updateMovement(float dt) {
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	if (glfwGetKey(window, 256 /*ESC*/) == 0) {
		glfwSetCursorPos(window, xRes / 2, yRes / 2);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	//Gets desired change in euler angles from change in mouse position. Mouse pos is set to the center each frame, so the change is just the dist from center
	eulerRot.x += glm::radians(sensitivity * dt * float(xRes / 2 - xPos));
	eulerRot.y -= glm::radians(sensitivity * dt * float(yRes / 2 - yPos));
	eulerRot.y = glm::clamp(eulerRot.y, -3.14f / 2.f, 3.14f / 2.f);

	//Rotation matrices based on input
	glm::mat3 yRot(
		1, 0, 0,
		0, cos(eulerRot.y), -sin(eulerRot.y),
		0, sin(eulerRot.y), cos(eulerRot.y)
	);
	glm::mat3 xRot(
		cos(eulerRot.x), 0, sin(eulerRot.x),
		0, 1, 0,
		-sin(eulerRot.x), 0, cos(eulerRot.x)
	);

	//Without this z rotation everything is upside down. May want to try to solve the root issue if it gives problems, but it works fine for now.
	glm::mat3 zRot(
		cos(3.14), -sin(3.14), 0,
		sin(3.14), cos(3.14), 0,
		0, 0, 1
	);

	glm::vec3 dir = normalize(glm::vec3(0, 0, -1) * xRot); //Order in which rotations are done is important. X goes first.
	glm::mat3 viewMatrix = glm::lookAt(
		position,
		position + dir,
		glm::vec3(0, 1, 0)	//X rotation is done with global up, rather than the up vector of the camera. This is because we want to avoid global z tilt.
	);

	viewMatrix = viewMatrix * zRot;
	viewMatrix = viewMatrix * yRot;

	glm::vec3 fwd = glm::vec3(0, 0, -1) * viewMatrix;
	glm::vec3 rgt = glm::cross(fwd, glm::vec3(0, 1, 0) * yRot);

	shader->setViewRot(viewMatrix);
	
	if (glfwGetKey(window, 87/*W*/)) {
		position += fwd * speed * dt;
	}
	if (glfwGetKey(window, 83/*S*/)) {
		position += -fwd * speed * dt;
	}
	if (glfwGetKey(window, 65/*A*/)) {
		position += rgt * speed * dt;
	}
	if (glfwGetKey(window, 68/*D*/)) {
		position += -rgt * speed * dt;
	}
	if (glfwGetKey(window, 32/*SPACE*/)) {
		position += -glm::vec3(0, 1, 0) * speed * dt;
	}
	if (glfwGetKey(window, 340/*LSHIFT*/)) {
		position += glm::vec3(0, 1, 0) * speed * dt;
	}
	shader->setViewPos(position);
}
