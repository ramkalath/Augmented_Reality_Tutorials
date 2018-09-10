/*****************************************************************************
 * Author : Ram
 * Date : 5/September/2018
 * Email : ramkalath@gmail.com
 * Breif Description : materials with finer specular lighting control
 * Detailed Description : Implementation of materials. Custom material - not taken from the material chart
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
#include <math.h>

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Lighting Properties", nullptr, nullptr);
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
	Shader lamp_shader("./shaders/lamp_vertex_shader.vert", "./shaders/lamp_fragment_shader.frag");

	// ================================================================================
	// data - vertices
	GLfloat vertices[] = 
	{
		//    vertices            Normals
		// front -------------------------------
		// face1, triangle1
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 

		// face1, triangle2
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 

		// back ---------------------------------
		// face2, triangle1
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

		// face2, triangle2
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

		// right -------------------------------
		// face3, triangle1
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 

		// face3, triangle2
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 

		// left --------------------------------
		// face4, triangle1
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 

		// face4, triangle2
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 

		// top ---------------------------------
		// face5, triangle1
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 

		// face5, triangle2
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 

		// bottom -------------------------------
		// face6, triangle1
		-0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 

		// face6, triangle2
		-0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f 
	};
	// ==================================================================================
	// Defining VAO and VBO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// unbind VAO
	glBindVertexArray(0);

	// ==================================================================================
	// Defining VAO and VBO for lamp
	GLuint VBO_lamp, VAO_lamp;
	glGenVertexArrays(1, &VAO_lamp);
	glGenBuffers(1, &VBO_lamp);
	glBindVertexArray(VAO_lamp);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lamp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// unbind VAO
	glBindVertexArray(0);
	
	// ================================================================================= 
	// defining the perspective projection matrix manually
	GLfloat angle = 45.0f;
	GLfloat n = 0.1f, f = 100.0f;
	GLfloat ar = (GLfloat)width/(GLfloat)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	// ================================================================================= 
	// material properties
	glm::vec3 material_ambient = glm::vec3(0.615f, 0.545f, 0.5515f);
	glm::vec3 material_diffuse = glm::vec3(0.7568f, 0.61424f, 0.7568f);
	glm::vec3 material_specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	float material_shininess = 0.01 * 128;

	// ================================================================================= 
	// light properties
	glm::vec3 light_ambient = glm::vec3(0.6f, 0.6f, 0.7f);
	glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);

	// ================================================================================= 
	// ambient light
	float ambient_strength = 0.3f;
	glm::vec3 ambient_light = ambient_strength * (light_ambient * material_ambient);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float time = glfwGetTime();
		float lamp_pos_x = 3*sin(time);
		float lamp_pos_z = 3*cos(time);

		// draw object ---------------------------------------------------------------
        glUseProgram(our_shader.program);
		glm::mat4 model = {1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		// ambient light
		glUniform3f(glGetUniformLocation(our_shader.program, "ambient_light"), ambient_light.x, ambient_light.y, ambient_light.z);

		// diffused light
		glUniform3f(glGetUniformLocation(our_shader.program, "lamp_pos"), lamp_pos_x, 0.0f, lamp_pos_z);
		glUniform3f(glGetUniformLocation(our_shader.program, "box_color"), 0.93f, 0.47f, 0.29f);
		glUniform3f(glGetUniformLocation(our_shader.program, "camera_pos"), 0.0f, 0.0f, 0.0f);

		// materials
		glUniform3f(glGetUniformLocation(our_shader.program, "material_diffuse"), material_diffuse.x, material_diffuse.y, material_diffuse.z);
		glUniform1f(glGetUniformLocation(our_shader.program, "material_shininess"), material_shininess*128);
		glUniform3f(glGetUniformLocation(our_shader.program, "material_specular"), material_specular.x, material_specular.y, material_specular.z);


		// lighting uniforms
		glUniform3f(glGetUniformLocation(our_shader.program, "light_diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
		glUniform3f(glGetUniformLocation(our_shader.program, "light_specular"), light_specular.x, light_specular.y, light_specular.z);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		// draw lamp --------------------------------------------------------------------
        glUseProgram(lamp_shader.program);

		glm::mat4 model_lamp = {0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, lamp_pos_x, 0.0f, lamp_pos_z, 1.0f};
		model_lamp = glm::rotate(model_lamp, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model_lamp = glm::rotate(model_lamp, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view_lamp = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_lamp));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glBindVertexArray(VAO_lamp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO_lamp);
	glDeleteBuffers(1, &VBO_lamp);
	glfwTerminate();
    return 0;
}

