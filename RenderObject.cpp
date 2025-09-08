#include "RenderObject.h"

void RenderObject::setupVertexArrays(GLfloat vertices[], int vSize, unsigned int indices[], int iSize) {
	indexCount = iSize / sizeof(indices[0]);


	glGenVertexArrays(1, &VAO);	//Always create the VAO before the VBO
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	//"Binds" our VBO object to the Array_Buffer, which is used for storing vertex info
	//Only one object can be bound to a target at a time. If OpenGL needs to use the array buffer for something, it'll direct it to our object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, &indices[0], GL_STATIC_DRAW);


	//Lets it know we have 3 values per vertex, and that they are floats. The last two arguments are memory space between vertices, and offset in memory.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


RenderObject::RenderObject(GLfloat vertices[], int vSize, unsigned int indices[], int iSize, Shader &_shader) {
	setupVertexArrays(vertices, vSize, indices, iSize);
	shader = &_shader;
}

RenderObject::~RenderObject() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void RenderObject::draw() {
	glUseProgram(shader->shaderProgram);
	glBindVertexArray(VAO);

	shader->Update();

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
}

