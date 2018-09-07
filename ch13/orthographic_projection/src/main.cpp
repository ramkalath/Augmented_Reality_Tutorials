/*****************************************************************************
 * Author : Ram
 * Date : 7/August/2018
 * Email : ramkalath@gmail.com
 * Breif Description : view and orthographic projection
 * Detailed Description : Implements view and orthographic matrix together.
 *****************************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "SOIL.h"
#include "../include/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width = 640, height = 800;

int main()
{
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "View and orthographic projection Matrices", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(window == nullptr)
    {
    	std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
    }
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, 800, 600);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	// ================================================================================

	// data - vertices ===================================================================
	GLfloat vertices[] = 
 	{
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	// ==================================================================================
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
	
	// =================================================================================
	// Texture Stuff
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image1 = SOIL_load_image("./resources/wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("./resources/batman.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(our_shader.program, "batman_texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(our_shader.program, "wood_texture"), 1);
		
        glUseProgram(our_shader.program);
		
		float time = glfwGetTime();
		glm::mat4 model = {1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, cos(time), -sin(time), 0.0f,
									 0.0f, sin(time),  cos(time), 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f};

		// compounding more rotations just to make it fancy
		model = glm::rotate(model, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		// defining the view matrix
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// for orthographic projection - manually defined
		GLfloat l = -1.5f, r = 1.5f;
		GLfloat b = -1.5f, t = 1.5f;
		GLfloat n = 0.1f, f = 100.0f;

		// defining the projection matrix
		glm::mat4 projection_orthographic = {2/(r-l), 0, 0, -(r+l)/(r-l),
											 0, 2/(t-b), 0, -(t+b)/(t-b),
											 0, 0, -2/(f-n), -(f+n)/(f-n),
											 0, 0, 0, 1};

		// OpenGL reads matrices in column major order
		projection_orthographic = glm::transpose(projection_orthographic);

		// for orthographic projection - glm fxn
		//glm::mat4 projection_orthographic = glm::ortho(l, r, b, t, n, f);

		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection_orthographic"), 1, GL_FALSE, glm::value_ptr(projection_orthographic));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
    return 0;
}

