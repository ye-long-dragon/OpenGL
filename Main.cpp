#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


GLfloat vertices[] =
{
	// Vertex coordinates         // Color values
	-0.5f, -0.5f, 0.0f,          0.8f, 0.3f, 0.5f,   // Lower-left corner
	0.5f, -0.5f, 0.0f,           0.34f, 0.0f, 0.0f,  // Lower-right corner (clamped)
	-0.5, 0.5f, 0.0f,            0.5f, 0.0f, .30f,   // Upper corner (clamped)
	0.5f, 0.5f, 0.0f,          0.0f, 0.47f, 0.76f,  // Middle-left corner (clamped)
	
};

GLuint indices[] = {
	0, 1, 2,
	1, 2, 3
};

int main() 
{
	//Initialize GLFW
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW we are using the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	

	//Create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "TestWindow", NULL, NULL);
	if (window == NULL)
	{
		//If the window creation fails, terminate GLFW and return an error code
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Make the window's context the current one
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	//Tell OpenGL the size of the rendering window
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0); // Position
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniId = glGetUniformLocation(shaderProgram.ID, "scale");
	
	//Texture
	int width, height, nrChannels;
	unsigned char* bytes = stbi_load("freddy.jpg", &width, &height, &nrChannels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);




	//Main loop
	while (!glfwWindowShouldClose(window))
	{		
		//Set the clear color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		//Use the shader program
		shaderProgram.Activate();
		glUniform1f(uniId, 0.5f);

		VAO1.Bind();

		//Draw the triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Swap the front and back buffers
		glfwSwapBuffers(window);

		//Poll for events from the user
		glfwPollEvents();
	}

	//Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	//Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}