#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
public:
	GLuint program;

	// constructor - actual definition of methods in shader.cpp in source directory
	Shader(const char* vertex_shader_path, const char* fragment_shader_path);
	~Shader();
};

#endif
