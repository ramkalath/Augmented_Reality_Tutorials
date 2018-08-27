/*****************************************************************************
 * Author : Ram
 * Date : 23/August/2018
 * Email : ramkalath@gmail.com
 * Breif Description : background less AR with a crate
 * Detailed Description : implements OpenCV to OpenGL perspective transform translation and thus background less AR with a crate
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

int width = 640, height = 480;

int main(int argc, char **argv)
{	
	cv::Mat rot_mat;

	// OpenGL initializations ---------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "View and perspective projection Matrices", nullptr, nullptr);
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
	glViewport(0, 0, width, height);

	Shader crate_shader("./shaders/crate_shader.vert", "./shaders/crate_shader.frag");
	Shader bg_shader("./shaders/bg_shader.vert", "./shaders/bg_shader.frag");

	// ARUCO marker tracking ---------------------------------------------
	if(argc < 3) // checks for argument list
	{
		cerr<<"Not enough arguments.\nUsage is : ./binary videodevice_number boardConfig.yml intrinsics.yml [size] [out]"<<endl;
		return 0;
	}

	cv::Mat intrinsic_matrix;					
	cv::Mat distortion_parameters;

	marker marker1(argv, intrinsic_matrix, distortion_parameters); // marker object is created and arguments are passed to the constructor
	cube cube1; // This is the object we have been drawing

	cv::Mat frame;
	cv::VideoCapture cap(atoi(argv[1]));

	// data - vertices_crate ===================================================================
	GLfloat vertices_crate[] = 
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
	
	// vertices and textures of the background
	GLfloat vertices_bg[] = 
	{
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f
	};

	// ==================================================================================
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_crate), vertices_crate, GL_STATIC_DRAW);
	
	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
	
	// ==================================================================================
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

	// =================================================================================
	// Texture Stuff
	GLuint texture1, texture2, texture_bg;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width_image, height_image;
	unsigned char* image1 = SOIL_load_image("./resources/wood.jpg", &width_image, &height_image, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image, height_image, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("./resources/batman.jpg", &width_image, &height_image, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image, height_image, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// =======================================================================
	// bg texture
	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	cap >> frame;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	// =======================================================================
	// crate presets ---------------------------------------------------------
	// lets make the crate bigger
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

	// default values of modelview
	glm::mat4 modelview;
	modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 10.0f));

	// defining the projection matrix
	float near = 0.1f;
	float far = 100.0f;
	float fx = intrinsic_matrix.at<float>(0,0);
	float fy = intrinsic_matrix.at<float>(1,1);
	float cx = intrinsic_matrix.at<float>(0,2);
	float cy = intrinsic_matrix.at<float>(1,2);

	glm::mat4 projection_perspective = {fx/cx, 0, 0, 0,
							  0, fy/cy, 0, 0,
							  0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near),
							  0, 0, -1, 0};

	projection_perspective = glm::transpose(projection_perspective);

	// =======================================================================
	// bg presets ------------------------------------------------------------
	glm::mat4 model_bg; 
	model_bg = glm::scale(model_bg, glm::vec3(109.0f, 95.0f, 1.0f));
	model_bg = glm::translate(model_bg, glm::vec3(0.0f, 0.0f, -99.0f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;
		marker1.marker_detect(frame);

		// there is a detect_flag variable which is of bool datatype and is set to 1 if the marker is detected.
		if(marker1.detect_flag)
		{
			frame = cube1.drawcube(frame, intrinsic_matrix, distortion_parameters, marker1.rvecs, marker1.tvecs); // Draws the cube
			cv::Rodrigues(marker1.rvecs, rot_mat);

			// defining the modelveiw matrix based on tvecs and rotation matrix
			modelview = {rot_mat.at<double>(0,0), rot_mat.at<double>(0,1), rot_mat.at<double>(0,2), marker1.tvecs.at<double>(0),
								   -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -marker1.tvecs.at<double>(1),
								   -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -marker1.tvecs.at<double>(2),
								   0.0f, 0.0f, 0.0f, 1.0f};
			modelview = glm::transpose(modelview);
		}

		cv::imshow("cubeoverlayed", frame);
		if(cv::waitKey(10)==27) break;  // if close is pressed on opencv window, exit the program

		// -------------------------------------------------------------------------------------
		// rendering 3d object and textures ----------------------------------------------------
		glUseProgram(crate_shader.program);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(crate_shader.program, "batman_texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(crate_shader.program, "wood_texture"), 1);

		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
		glUniformMatrix4fv(glGetUniformLocation(crate_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		// -------------------------------------------------------------------------------------

		// rendering background object and textures ---------------------------------------------
		glUseProgram(bg_shader.program);
		glBindVertexArray(VAO_bg);

		cv::flip(frame, frame, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glUniform1i(glGetUniformLocation(bg_shader.program, "webcam_texture"), 0);

		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//--------------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}
