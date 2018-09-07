/*****************************************************************************
 * Author : Ram
 * Date : 28/July/2018
 * Email : ramkalath@gmail.com
 * Breif Description : mixture of two textures
 * Detailed Description : Implements a mixture of two textures.
 *****************************************************************************/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shader.h"
#include <SOIL.h>

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, thus closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        cout << "Window is closing because u pressed the ESC key" << endl;
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Mixture of Textures", nullptr, nullptr);
    if(window == nullptr)
    {
        cout << "Failed to create a GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initializing glew -------------------------------------------------------------------
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetKeyCallback(window, key_callback);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

    //----------------------------------------------------------------------------------------
    // let us now write code for the actual data points for the triangle and add bind it with a VBO. VAO is then used to encapsulate VBO
							// vertex pos        texture coords
    GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left bottom
							0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// right bottom
							0.0f,  0.5f, 0.0f, 0.5f, 1.0f};  // top

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// attribute 0 vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	// attribute 1 texture_coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);               // unbinding VAO
    //---------------------------------------------------------------------------------------
	// Texture - arch
	GLuint arch_texture; // create a texture object; just like vao, vbo, ebo etc
	glGenTextures(1, &arch_texture); // generate textures
	glBindTexture(GL_TEXTURE_2D, arch_texture); // Bind texture; all usual procedure

	// what kind of wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// interpolation type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// actual loading of image
	int width, height;
	unsigned char* image = SOIL_load_image("./resources/arch.jpg", &width, &height, 0, SOIL_LOAD_RGB);
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

    // Game Loop ---------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ------------------------------------------------
        glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// Binding our first texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, arch_texture);
		glUniform1i(glGetUniformLocation(our_shader.program, "arch_texture"), 0);

		// Binding our second texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wood_texture);
		glUniform1i(glGetUniformLocation(our_shader.program, "wood_texture"), 1);

        // Draw a rectangle
        glUseProgram(our_shader.program);
        glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
