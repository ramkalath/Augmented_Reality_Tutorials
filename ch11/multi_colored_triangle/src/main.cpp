/*****************************************************************************
 * Author : Ram
 * Date : 20/July/2018
 * Email : ramkalath@gmail.com
 * Breif Description : multicolored triangle
 * Detailed Description : This program creates a multicolored triangle
 *****************************************************************************/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shader.h"

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Multicolored Triangle", nullptr, nullptr);
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
							// vertex pos        vertex colors
    GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // left bottom
							0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// right bottom
							0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f};  // top

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// attribute 0 vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	// attribute 1 vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);               // unbinding VAO

    //---------------------------------------------------------------------------------------
    // Game Loop ---------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ------------------------------------------------
        glfwPollEvents();

        glClearColor(0.09f, 0.105f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
