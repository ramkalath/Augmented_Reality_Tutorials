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

    GLFWwindow *window = glfwCreateWindow(width_window, height_window, "AR with a crate", nullptr, nullptr);
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

	Shader crate_shader("./shaders/crate_vertex_shader.vert", "./shaders/crate_fragment_shader.frag");
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
	// Crate object definitions
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
	
	// -----------------------------------------------------------------------------------------------
	// vao and vbo objects of crate
	GLuint VBO_crate, VAO_crate;
	glGenVertexArrays(1, &VAO_crate);
	glGenBuffers(1, &VBO_crate);

	glBindVertexArray(VAO_crate);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_crate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
	
	// -----------------------------------------------------------------------------------------------
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

 	// =========================================================================================================
	// crate transformations presets (scaling, default modelview and perspective projection)
	glm::mat4 model_crate;
	model_crate = glm::scale(model_crate, glm::vec3(5.0f, 5.0f, 5.0f));
	model_crate = glm::translate(model_crate, glm::vec3(0.5f, 0.5f, -0.5f));

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

 	// =========================================================================================================
	// background object definitions
	GLfloat vertices_bg[] = 
	{
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
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
	width = frame.size().width;
	height = frame.size().height;

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
	// transformation presets for the background object (model, view and orthographic projection)
	glm::mat4 model_bg;
	glm::mat4 view_bg;
	GLfloat ortho_far = 10000.0f;

	model_bg = glm::scale(model_bg, glm::vec3(width_window, height_window, 1));
	view_bg = glm::translate(view_bg, glm::vec3(0, 0, -ortho_far));

	// we will use orthographic projection for the background
	glm::mat4 orthographic_projection_bg = glm::ortho(0.0f, (GLfloat)width_window, 0.0f, (GLfloat)height_window, near, ortho_far);
 	// =========================================================================================================

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;
		marker1.marker_detect(frame);
		glm::mat4 modelview_crate = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1};

		// there is a detect_flag variable which is of bool datatype and is set to 1 if the marker is detected.
		if(marker1.detect_flag)
		{
			cv::Rodrigues(marker1.rvecs, rot_mat);
			// defining the modelveiw matrix based on tvecs and rotation matrix
			modelview_crate = {rot_mat.at<double>(0,0), rot_mat.at<double>(0,1), rot_mat.at<double>(0,2), marker1.tvecs.at<double>(0), -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -marker1.tvecs.at<double>(1), -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -marker1.tvecs.at<double>(2), 0.0f, 0.0f, 0.0f, 1.0f};
		}
		modelview_crate = glm::transpose(modelview_crate);

		// draw crate ------------------------------------------------------------------------------------------
		glUseProgram(crate_shader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(crate_shader.program, "batman_texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(crate_shader.program, "wood_texture"), 1);
		
		glBindVertexArray(VAO_crate);

		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "model_crate"), 1, GL_FALSE, glm::value_ptr(model_crate));
		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "modelview_crate"), 1, GL_FALSE, glm::value_ptr(modelview_crate));
		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "perspective_projection_crate"), 1, GL_FALSE, glm::value_ptr(perspective_projection_crate));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw bg ---------------------------------------------------------------------------------------------
		glUseProgram(bg_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "model_bg"), 1, GL_FALSE, glm::value_ptr(model_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "view_bg"), 1, GL_FALSE, glm::value_ptr(view_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "orthographic_projection_bg"), 1, GL_FALSE, glm::value_ptr(orthographic_projection_bg));

		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bg_shader.program, "webcam_texture"), 2);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		// -----------------------------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_crate);
	glDeleteBuffers(1, &VBO_crate);

	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);

	glfwTerminate();
	return EXIT_SUCCESS;
}
