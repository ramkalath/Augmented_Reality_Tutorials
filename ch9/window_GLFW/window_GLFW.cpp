/*****************************************************************************
 * Author : Ram
 * Date : 18/June/2018
 * Email : ramkalath@gmail.com
 * Breif Description : GLFW window creation
 * Detailed Description : type 'make' to compile and 'make run' to execute
 *****************************************************************************/
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// call back function
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    glfwInit();		// Initialize GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// Describe the major version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// Describe the minor version of OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// Use the core profile
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);		// make sure the window is not resizable

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My first GLFW window", nullptr, nullptr);		// create a window of size 800, 600
    if(window == nullptr)
    {
        std::cerr << "failed to create a GLFW window" << std::endl;		// print out saying that GLFW has failed to initialize
        glfwTerminate();	// stop GLFW
        return -1;		// return -1 to the program terminating execution of the remaining program
    }
    glfwMakeContextCurrent(window);		// context is something like a handle of entire OpenGL's operations and states.
    glfwSetKeyCallback(window, key_callback);	// associating the key_callback() function to control the glfw window.

    // some glew initializations
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    { // check for fails
        std::cerr << "Unable to initialize GLEW" << std::endl;
        return -1;
    }

    // setting the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    while(!glfwWindowShouldClose(window))		// until the window recieves a termination signal continue the loop
    {
        glfwPollEvents();	// check for termination events such as esc press or x press on the dialogue window
        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);	// set the window color
        glClear(GL_COLOR_BUFFER_BIT);	// clear the color buffer
        glfwSwapBuffers(window);	// Swap the buffer
    }

    glfwTerminate();	// Close and free up all the resourses used
    return EXIT_SUCCESS;
}
