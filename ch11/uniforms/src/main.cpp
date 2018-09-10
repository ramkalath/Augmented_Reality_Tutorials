/*****************************************************************************
 * Author : Ram
 * Date : 18/July/2018
 * Email : ramkalath@gmail.com
 * Breif Description : implements uniforms to pass green color value
 * Detailed Description : implements uniforms to pass green color value. The tone of green changes with time.
 *****************************************************************************/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shader.h"
#include <math.h>

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Uniforms", nullptr, nullptr);
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
    // let us now write code for the actual data points for the rectangle and add bind it with a VBO. VAO is then used to encapsulate VBO
    GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,   // left bottom 
                           -0.5f,  0.5f, 0.0f,   // left top 
							0.5f, -0.5f, 0.0f,   // right bottom 

                           -0.5f,  0.5f, 0.0f,   // left top 
                            0.5f,  0.5f, 0.0f,   // right top      
							0.5f, -0.5f, 0.0f};  // right bottom 

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);               // unbinding VAO

    //---------------------------------------------------------------------------------------
	GLint color_location = glGetUniformLocation(our_shader.program, "u_color");   // this finds the location of the uniform -> ourColor
    // Game Loop ---------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ------------------------------------------------
        glfwPollEvents();

        glClearColor(0.09f, 0.105f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// change color value of green
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue)/2) + 0.5;

        // Draw a rectangle
        glUseProgram(our_shader.program);
        glUniform4f(color_location, 0.0f, greenValue, 0.0f, 1.0f);     // we use this function to update the found vertexColorLocation's colour value to a varying green value
        glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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
