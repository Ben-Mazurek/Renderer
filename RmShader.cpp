#include "RmShader.h"


RmShader::RmShader(int screenWidth, int screenHeight) : Shader("Shaders/vertex.vert", "Shaders/fragment.frag") {
	startTime = clock();
	glUseProgram(shaderProgram);
	GLint loc = glGetUniformLocation(shaderProgram, "iResolution");
	glUniform2f(loc, screenWidth, screenHeight);

	PhongMat sphereMat = {
		glm::vec4(0.2, 0.2, 0.2, 0),
		glm::vec4(0.7, 0.2, 0.2, 0),
		glm::vec4(1.000,1.000,1.000, 0),
		glm::vec4(0.133, 0.592, 0.749, 0),
		10.
	};
	PhongMat planeMat = {
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
		10.
	};

	PhongMat mats[2] = { sphereMat, planeMat };
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mats), &mats[0], GL_DYNAMIC_DRAW);
	GLuint loc2 = glGetUniformBlockIndex(shaderProgram, "PhongMats");
	std::cout << loc2;
	glBindBufferBase(GL_UNIFORM_BUFFER, loc2, UBO);

}

void RmShader::Update() {
	float runningTime = (clock() - startTime)/CLOCKS_PER_SEC;
	glUseProgram(shaderProgram);
	GLint loc = glGetUniformLocation(shaderProgram, "iTime");
	glUniform1f(loc, runningTime);
}

void RmShader::setViewRot(glm::mat3 matrix) {
	glUseProgram(shaderProgram);
	GLint loc = glGetUniformLocation(shaderProgram, "viewDir"); 
	glUniformMatrix3fv(loc, 1, true, glm::value_ptr(matrix));
}

void RmShader::setViewPos(glm::vec3 pos) {
	glUseProgram(shaderProgram);
	GLint loc = glGetUniformLocation(shaderProgram, "eye");
	glUniform3fv(loc, 1, glm::value_ptr(pos));
}