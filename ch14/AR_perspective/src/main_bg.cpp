/*****************************************************************************
 * Author : Ram
 * Date : 28/August/2018
 * Email : ramkalath@gmail.com
 * Breif Description : AR with bg as orthographic projection
 * Detailed Description : implements complete AR with the model of a crate. Crate model has perspective projection and background has orthographic projection. To run this file type './run.sh'
 *****************************************************************************/

// usual imports
#include <iostream>
#include <fstream>
#include <sstream>

// marker tracking 
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h> 
#include "../include/marker_detect.h"
#include "../include/cube.h"

// OpenGL stuff
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL.h"
#include "../include/shader.h"

// some extra required imports
#include <strings.h>
#include <vector>
#include <math.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width_window = 640, height_window = 480;

int main(int argc, char **argv)
{	
 	// =========================================================================================================
	// OpenGL initializations
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(width_window, height_window, "View and perspective projection Matrices", nullptr, nullptr);
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
	glfwGetFramebufferSize(window, &width_window, &height_window);
	glViewport(0, 0, width_window, height_window);

	Shader bg_shader("./shaders/bg_vertex_shader.vert", "./shaders/bg_fragment_shader.frag");

 	// =========================================================================================================
	// ARUCO initializations
	if(argc < 3) // checks for argument list
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number boardConfig.yml intrinsics.yml [size] [out]"<<endl;
		return 0;
	}

	cv::Mat frame;
	cv::Mat rot_mat;
	cv::Mat intrinsic_matrix;					
	cv::Mat distortion_parameters;

	marker marker1(argv, intrinsic_matrix, distortion_parameters); // marker object is created and arguments are passed to the constructor
	cv::VideoCapture cap(atoi(argv[1]));
 
 	// =========================================================================================================
	// background object definitions
	GLfloat vertices_bg[] = 
	{
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	};

	// -----------------------------------------------------------------------------------------------
	// background object
	GLuint VBO_bg, VAO_bg;
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);

	glBindVertexArray(VAO_bg);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);

	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO_bg

	// -----------------------------------------------------------------------------------------------
	// webcam texture
	cap >> frame;
	int width = frame.size().width;
	int height = frame.size().height;

	GLuint texture_bg;
	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
 	// =========================================================================================================
	// defining the projection matrix
	float near = 0.1f;
	float far = 500.0f;
	float fx = intrinsic_matrix.at<float>(0,0);
	float fy = intrinsic_matrix.at<float>(1,1);
	float cx = intrinsic_matrix.at<float>(0,2);
	float cy = intrinsic_matrix.at<float>(1,2);

	glm::mat4 perspective_projection_crate = {fx/cx, 0, 0, 0,
							  0, fy/cy, 0, 0,
							  0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near),
							  0, 0, -1, 0};

	perspective_projection_crate = glm::transpose(perspective_projection_crate);

	glm::mat4 modelview_bg = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -499, 0, 0, 0, 1};
	modelview_bg = glm::transpose(modelview_bg);
	modelview_bg = glm::scale(modelview_bg, glm::vec3(499*cx/fx, 499*cy/fy, 0));
 	// =========================================================================================================

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;
		marker1.marker_detect(frame);
		// draw bg ---------------------------------------------------------------------------------------------
		glUseProgram(bg_shader.program);
		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bg_shader.program, "webcam_texture"), 2);

		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(perspective_projection_crate));

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		// -----------------------------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);

	glfwTerminate();
	return EXIT_SUCCESS;
}
