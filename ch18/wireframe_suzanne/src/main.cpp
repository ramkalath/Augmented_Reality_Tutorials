/*****************************************************************************
 * Author : Ram
 * Date : 8/September/2018
 * Email : ramkalath@gmail.com
 * Breif Description : rotating wireframe model of suzanne
 * Detailed Description : rotating wireframe model of suzanne
 *****************************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shader.h"
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>
#include <vector>
#include <iostream>

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	int width = 800;
	int height = 600;
    // initializing glfw -------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "wiremesh of suzanne", nullptr, nullptr);
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
	// Assimp importer ====================================================================
	Assimp::Importer importer;
	const std::string& path = "./resources/suzanne.obj"; 
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return 0;
	}

	std::cout << scene->mNumMeshes << std::endl;
	// grabbing vertices from assimp data structures ----------------------------------------
	std::vector<glm::vec3> vertices;
	for(unsigned int num_meshes=0; num_meshes<scene->mNumMeshes; num_meshes++){
		for(unsigned int num_vertices_per_mesh=0; num_vertices_per_mesh<scene->mMeshes[num_meshes]->mNumVertices; num_vertices_per_mesh++){
			glm::vec3 a(scene->mMeshes[num_meshes]->mVertices[num_vertices_per_mesh].x, scene->mMeshes[num_meshes]->mVertices[num_vertices_per_mesh].y, scene->mMeshes[num_meshes]->mVertices[num_vertices_per_mesh].z);
			vertices.push_back(a);
		}
	}
	GLfloat *vertices_array = &vertices[0].x;
	GLuint size_of_vertices_array = vertices.size() * 3 * sizeof(GLfloat); // vertices data size * x,y,z values per vertex * sizeof(GLfloat)

	// ---------------------------------------------------------------------------------------
	// grabbing indices from assimp data structures ------------------------------------------
	//std::cout << scene->mMeshes[0]->mFaces[0].mIndices[0] << std::endl; // this is how you access the first index of the first face of the first mesh of the scene.
	std::vector<GLuint> indices; 
	for(unsigned int i=0; i<scene->mNumMeshes; i++)
		for(unsigned int j=0; j<scene->mMeshes[i]->mNumFaces; j++){
			indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
			indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
			indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]); 
		}
	GLuint *indices_array = indices.data();
	GLuint size_of_indices_array = indices.size()*sizeof(GLuint);
	// ---------------------------------------------------------------------------------------

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_of_vertices_array, vertices_array, GL_STATIC_DRAW);

	// attribute 0 vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indices_array, indices_array, GL_STATIC_DRAW);

    glBindVertexArray(0);               // unbinding VAO
    //---------------------------------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ------------------------------------------------
        glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = sin(glfwGetTime());
		glm::mat4 model = glm::mat4(1.0f); 

		model = glm::rotate(model, glm::radians(time*90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(time*90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection_perspective = glm::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

        // Draw a rectangle
        glUseProgram(our_shader.program);
        glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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
