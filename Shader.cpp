#include "Shader.h"


std::string Shader::readFromFile(std::string path) {

		std::ifstream file;
		file.open(path);
		std::string results((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); //https://stackoverflow.com/questions/18398167/how-to-copy-a-txt-file-to-a-char-array-in-c
		file.close();
		return results;
}

Shader::Shader(std::string vertPath, std::string fragPath) {
		//Creates a reference value to the shader that will be created.
	    //It's type is an int, and OpenGL will use that to find the actual object somewhere. This means we can only interact with it through OpenGL API functions, which is good actually.
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//Loads the source code string we have into the shader object
		std::string vs = readFromFile(vertPath);
		const char* vertSource = &vs[0];
		glShaderSource(vertexShader, 1, &vertSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string fs = readFromFile(fragPath);
		const char* fragSource = &fs[0];

		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);

		//Creates a "shader program" which I think is how OpenGL gets shaders to the GPU
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		//Links the shader program, I think this links the shaders allowing them to send stuff like vertex data along
		glLinkProgram(shaderProgram);

		//Now that shaders are sent over to the shader program we don't need them here anymore
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glUseProgram(shaderProgram);
	}

	Shader::~Shader() {
		glDeleteProgram(shaderProgram);
	}

	void Shader::Update() {
		std::cout << "p";
	}

