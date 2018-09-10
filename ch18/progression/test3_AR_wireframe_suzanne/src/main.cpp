/*****************************************************************************
 * Author : Ram
 * Date : 9/September/2018
 * Email : ramkalath@gmail.com
 * Breif Description : AR box with lighting effects
 * Detailed Description : progression for AR suzanne. Implements lighting on a box with AR.
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

#include <SOIL.h>
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

    GLFWwindow *window = glfwCreateWindow(width_window, height_window, "AR with a suzanne", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(window == nullptr){
    	std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
    }
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width_window, &height_window);
	glViewport(0, 0, width_window, height_window);

 	// =========================================================================================================
	// ARUCO initializations
	if(argc < 3){
		std::cerr << "Not enough arguments.\nUsage is : ./binary videodevice_number boardConfig.yml intrinsics.yml [size] [out]" << std::endl;
		return 0;
	}
	cv::Mat frame;
	cv::Mat rot_mat;
	cv::Mat intrinsic_matrix;					
	cv::Mat distortion_parameters;
	marker marker1(argv, intrinsic_matrix, distortion_parameters); // marker object is created and arguments are passed to the constructor
	cv::VideoCapture cap(atoi(argv[1]));
 	// =========================================================================================================
	Shader suzanne_shader("./shaders/suzanne_vertex_shader.vert", "./shaders/suzanne_fragment_shader.frag");
	Shader bg_shader("./shaders/bg_vertex_shader.vert", "./shaders/bg_fragment_shader.frag");
	Shader lamp_shader("./shaders/lamp_vertex_shader.vert", "./shaders/lamp_fragment_shader.frag");

 	// =========================================================================================================
	// data - vertices
	GLfloat vertices_suzanne[] = {
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f, -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f,  0.5f,  0.5f,  0.0f, -1.0f,  0.0f };
	// -----------------------------------------------------------------------------------------------
	// Defining VAO and VBO
	GLuint VBO_suzanne, VAO_suzanne;
	glGenVertexArrays(1, &VAO_suzanne);
	glGenBuffers(1, &VBO_suzanne);
	glBindVertexArray(VAO_suzanne);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_suzanne);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_suzanne), vertices_suzanne, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	// -----------------------------------------------------------------------------------------------
	// suzanne transformations presets (scaling, default modelview and perspective projection)
	glm::mat4 model_suzanne;
	model_suzanne= glm::scale(model_suzanne, glm::vec3(5.0f, 5.0f, 5.0f));
	model_suzanne= glm::translate(model_suzanne, glm::vec3(0.5f, 0.5f, -0.5f));
	float near = 0.1f;
	float far = 500.0f;
	float fx = intrinsic_matrix.at<float>(0,0);
	float fy = intrinsic_matrix.at<float>(1,1);
	float cx = intrinsic_matrix.at<float>(0,2);
	float cy = intrinsic_matrix.at<float>(1,2);
	glm::mat4 perspective_projection = {fx/cx, 0, 0, 0, 0, fy/cy, 0, 0, 0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near), 0, 0, -1, 0};
	perspective_projection = glm::transpose(perspective_projection);

 	// =========================================================================================================
	// lamp object definitions
	GLfloat lamp_vertices[] = {
	-0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f};
	// -----------------------------------------------------------------------------------------------
	GLuint VBO_lamp, VAO_lamp;
	glGenVertexArrays(1, &VAO_lamp);
	glGenBuffers(1, &VBO_lamp);
	glBindVertexArray(VAO_lamp);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lamp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lamp_vertices), lamp_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

 	// =========================================================================================================
	// background object definitions
	GLfloat vertices_bg[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.0f,  0.0f, 0.0f};
	// -----------------------------------------------------------------------------------------------
	// background object
	GLuint VBO_bg, VAO_bg;
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);
	glBindVertexArray(VAO_bg);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
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
	// -----------------------------------------------------------------------------------------------
	// transformation presets for the background object (model, view and orthographic projection)
	glm::mat4 modelview_bg = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -499, 0, 0, 0, 1};
	modelview_bg = glm::transpose(modelview_bg);
	modelview_bg = glm::scale(modelview_bg, glm::vec3(998*cx/fx, 998*cy/fy, 0));
 	// =========================================================================================================
	// ambient light
	float ambient_strength = 0.3f;
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient_light = ambient_strength * light_color;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;
		marker1.marker_detect(frame);
		glm::mat4 modelview_suzanne= {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1};
		if(marker1.detect_flag)
		{
			cv::Rodrigues(marker1.rvecs, rot_mat);
			modelview_suzanne = {rot_mat.at<double>(0,0), rot_mat.at<double>(0,1), rot_mat.at<double>(0,2), marker1.tvecs.at<double>(0), -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -marker1.tvecs.at<double>(1), -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -marker1.tvecs.at<double>(2), 0.0f, 0.0f, 0.0f, 1.0f};
		}
		modelview_suzanne = glm::transpose(modelview_suzanne);

		float time = glfwGetTime();
		float lamp_pos_x = 9*sin(time);
		float lamp_pos_y = 9*cos(time);

		// draw suzanne ------------------------------------------------------------------------------------------
		glUseProgram(suzanne_shader.program);
		glBindVertexArray(VAO_suzanne);
		glUniformMatrix4fv(glGetUniformLocation(suzanne_shader.program, "model_suzanne"), 1, GL_FALSE, glm::value_ptr(model_suzanne));
		glUniformMatrix4fv(glGetUniformLocation(suzanne_shader.program, "modelview_suzanne"), 1, GL_FALSE, glm::value_ptr(modelview_suzanne));
		glUniformMatrix4fv(glGetUniformLocation(suzanne_shader.program, "perspective_projection_suzanne"), 1, GL_FALSE, glm::value_ptr(perspective_projection));
		glUniform3f(glGetUniformLocation(suzanne_shader.program, "ambient_light"), ambient_light.x, ambient_light.y, ambient_light.z);
		glUniform3f(glGetUniformLocation(suzanne_shader.program, "lamp_pos"), lamp_pos_x, lamp_pos_y, -3.0f);
		glUniform3f(glGetUniformLocation(suzanne_shader.program, "light_color"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(suzanne_shader.program, "box_color"), 0.93f, 0.47f, 0.29f);
		glUniform3f(glGetUniformLocation(suzanne_shader.program, "camera_pos"), 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw bg ---------------------------------------------------------------------------------------------
		glUseProgram(bg_shader.program);
		glBindVertexArray(VAO_bg);
		glActiveTexture(GL_TEXTURE0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bg_shader.program, "webcam_texture"), 0);
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(perspective_projection));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// draw lamp ------------------------------------------------------------------------------------------
		glUseProgram(lamp_shader.program);
		glBindVertexArray(VAO_lamp);
		glm::mat4 model_lamp = {2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, lamp_pos_x + 2.5f, lamp_pos_y + 2.5f, -3.0f, 1.0f}; // we shifted the 3d model by 0.5 and then scaled it by 5. A total of 2.5 units worth shift has to be provided to the lamp so that it rotates around the object well.
		model_lamp = glm::rotate(model_lamp, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model_lamp = glm::rotate(model_lamp, glm::radians(sin(time)*90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "model_lamp"), 1, GL_FALSE, glm::value_ptr(model_lamp));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "modelview_lamp"), 1, GL_FALSE, glm::value_ptr(modelview_suzanne));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "projection_lamp"), 1, GL_FALSE, glm::value_ptr(perspective_projection));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_suzanne);
	glDeleteBuffers(1, &VBO_suzanne);
	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);
	glDeleteVertexArrays(1, &VAO_lamp);
	glDeleteBuffers(1, &VBO_lamp);

	glfwTerminate();
	return EXIT_SUCCESS;
}
