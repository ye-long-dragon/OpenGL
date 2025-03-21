#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



int main() 
{
	//Initialize GLFW
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW we are using the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a vertex array object
	GLfloat vertices[] = {
		-0.5f, -0.5f*float(sqrt(3))/3, 0.0f, //Bottom-left vertex
		0.5f, -0.5f * float(sqrt(3)) /3 , 0.0f, //Bottom-right vertex
		0.0f, 0.5f * float(sqrt(3)) *2/ 3 , 0.0f //Top vertex
		- 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Middle-left vertex
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Middle-right vertex
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f //Bottom vertex
	};

	GLuint indices[] = {
		0, 3, 5, //Lower-left triangle
		3, 2, 4, //Lower-right triangle
		5, 4, 1 //Upper triangle
	};

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
	glViewport(0, 0, 800, 600);

	//Create a vertex buffer object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attach the shader source code to the shader object as a function from string
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the shader
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Create a shader program
	GLuint shaderProgram = glCreateProgram();

	//Attach the vertex and fragment shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link the shaders in the program
	glLinkProgram(shaderProgram);
	//Activate the shader program
	glDeleteShader(vertexShader);	
	glDeleteShader(fragmentShader);


	//Create a vertex buffer object
	GLuint VBO,VAO,EBO;
	//Generate the VAO and VBO with the corresponding ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind the VAO first, then bind and set the vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	//	Set the clear color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{		
		//Set the clear color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		//Use the shader program
		glUseProgram(shaderProgram);
		//Draw the triangle
		glBindVertexArray(VAO);
		//Draw the triangle
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//Swap the front and back buffers
		glfwSwapBuffers(window);

		//Poll for events from the user
		glfwPollEvents();
	}

	//Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}