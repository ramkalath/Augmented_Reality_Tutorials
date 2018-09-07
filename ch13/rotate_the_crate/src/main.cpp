/*****************************************************************************
 * Author : Ram
 * Date : 3/August/2018
 * Email : ramkalath@gmail.com
 * Breif Description : Model matrix on the box to make it larger and to impose a rotation
 * Detailed Description : We explore a model matrix in this section that resizes the box and rotates it as its being kept in the world space.
 *****************************************************************************/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shader.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, thus closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main()
{
    // initializing glfw -------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "rotating box", nullptr, nullptr);
    if(window == nullptr)
    {
		std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initializing glew -------------------------------------------------------------------
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
		std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
	glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);
    glfwSetKeyCallback(window, key_callback);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

    //----------------------------------------------------------------------------------------
    // Cube has 6 faces and each face has 4 vertices
							// vertex pos        texture coords
    GLfloat vertices[] = {			// Front face
						   -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // left bottom        0
							0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // right bottom       1
						   -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // left top           2
							0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // right top          3

									// back face
						   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // left bottom        4
							0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // right bottom       5
						   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // left top           6
							0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // right top          7

									// left side face
						   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // left bottom        8
						   -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // right bottom       9
						   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // left top           10
						   -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // right top          11

						   			// right side face
						    0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // left bottom        12
						    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // right bottom       13
						    0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // left top           14
						    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // right top          15

									// bottom face
						   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // left bottom        16
						    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // right bottom       17
						   -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // left top           18
						    0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // right top          19

									// top face
						   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // left bottom        20
						    0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // right bottom       21
						   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // left top           22
						    0.5f,  0.5f, -0.5f, 1.0f, 1.0f  // right top          23
	};

	GLuint indices[] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 8, 9, 10, 9, 10, 11, 12, 13, 14, 13, 14, 15, 16, 17, 18, 17, 18, 19, 20, 21, 22, 21, 22, 23};

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// attribute 0 vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	// attribute 1 texture_coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);               // unbinding VAO
    //---------------------------------------------------------------------------------------
	// Texture - batman
	GLuint batman_texture; // create a texture object; just like vao, vbo, ebo etc
	glGenTextures(1, &batman_texture); // generate textures
	glBindTexture(GL_TEXTURE_2D, batman_texture); // Bind texture; all usual procedure

	// what kind of wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// interpolation type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// actual loading of image
	int width, height;
	unsigned char* image = SOIL_load_image("./resources/batman.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // convert image to texture
	glGenerateMipmap(GL_TEXTURE_2D); // generate mipmap
	SOIL_free_image_data(image); // free image data from memory
	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

    //---------------------------------------------------------------------------------------
	// Texture - wood
	GLuint wood_texture; // create a texture object; just like vao, vbo, ebo etc
	glGenTextures(1, &wood_texture); // generate textures
	glBindTexture(GL_TEXTURE_2D, wood_texture); // Bind texture; all usual procedure

	// what kind of wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// interpolation type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// actual loading of image
	image = SOIL_load_image("./resources/wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // convert image to texture
	glGenerateMipmap(GL_TEXTURE_2D); // generate mipmap
	SOIL_free_image_data(image); // free image data from memory
	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture
    //---------------------------------------------------------------------------------------
	// Let us create a basic model matrix
	// notice the first 3 elements of the leading diagonal is 0.5 which reduces the size of the crate along each axis by half
	glm::mat4 half_size_matrix = {0.5f, 0.0f, 0.0f, 0.0f,
								  0.0f, 0.5f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.5f, 0.0f,
								  0.0f, 0.0f, 0.0f, 1.0f};

	// find the location of the matrix in the shader
	GLuint half_size_location = glGetUniformLocation(our_shader.program, "half_size");
	glUniformMatrix4fv(half_size_location, 1, GL_FALSE, glm::value_ptr(half_size_matrix));

    // Game Loop ---------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ------------------------------------------------
        glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Binding our first texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, batman_texture);
		glUniform1i(glGetUniformLocation(our_shader.program, "batman_texture"), 0);

		// Binding our second texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wood_texture);
		glUniform1i(glGetUniformLocation(our_shader.program, "wood_texture"), 1);

		// half size matrix
		glm::mat4 half_size_matrix = {1.0f, 0.0f, 0.0f, 0.0f,
									  0.0f, 1.0f, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  0.0f, 0.0f, 0.0f, 1.0f};

		// find the location of the half_size matrix in the shader
		GLuint half_size_location = glGetUniformLocation(our_shader.program, "half_size");
		glUniformMatrix4fv(half_size_location, 1, GL_FALSE, glm::value_ptr(half_size_matrix));

		// Rotation matrix
		// lets create the rotation matrix
		float time = glfwGetTime();
		glm::mat4 rotation_matrix = {1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, cos(time), -sin(time), 0.0f,
									 0.0f, sin(time),  cos(time), 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f};

		GLuint rotation_matrix_location = glGetUniformLocation(our_shader.program, "rotation");
		glUniformMatrix4fv(rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(rotation_matrix));

        // Draw a rectangle
        glUseProgram(our_shader.program);
        glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
     }
    // Deleting the used resources --------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // --------------------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
