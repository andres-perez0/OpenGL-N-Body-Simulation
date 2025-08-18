#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#define SCR_HEIGHT 800
#define SCR_WIDTH 1000

#include "shaderClass.h"

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main() {
	// Initializes GlFLW
	glfwInit();

	// tells GLFW what version of OpenGl we're using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tells GLFW that we're using the CORE profile (modern features)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates a GLFWwindow object and checks if it failed to create
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learningAgain", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduces the window into the current context
	glfwMakeContextCurrent(window); 

	// Loads GLAD so it configures Open GL
	gladLoadGL();
	// specifies the viewport of OpenGL
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Create vertex shader object using the reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// The computer cannot read source code so we have to compile to machine code
	glCompileShader(vertexShader);

	// Similar process for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create the shader program - remember every gl object utilizes a reference
	GLuint shaderProgram = glCreateProgram();
	// Attach your shader to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link all the shaders together into the shader program
	glLinkProgram(shaderProgram);

	// Delete now useless shader objects);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Shader Class
	//Shader shaderProgram("default.vert", "default.frag");

	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3 , 0.0f,	// Lower Left Corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		// Lower Right Corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	// Upper Corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f		// Inner down
	};

	GLuint indices[] = {
		0, 3, 5,	// Lower Left Triangle
		3, 2, 4,	// Lower Right Triangle
		5, 4, 1,	// Upper Triangle
	};
	// Generate the reference for the following
	GLuint VAO, VBO, EBO; 
	// Vertex Array Object 
	// Vertex Buffer Object 
	// Element Buffer Object
	// Genertae the VAO and VBO with one object each
	glGenVertexArrays(1, &VAO); //order is critical
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Make the reference VAO into the current VAO by binding the object
	glBindVertexArray(VAO);
	// Bind the VBO, specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices inot the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Binds the element array object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Configure the vertex attribute so that OpenGL knows how to read the VBO
	// pos, # of values, type, FALSE, STRIDE, (void *) 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex attribute so that open gl knows to use it
	glEnableVertexAttribArray(0);
	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBOwe created
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// EBO

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//shaderProgram.Activate();
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGl knows to use it
		glBindVertexArray(VAO);
		// primate | indices we want to draw | dt of indices | index of our indices
		glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteShader(shaderProgram);
	//shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}