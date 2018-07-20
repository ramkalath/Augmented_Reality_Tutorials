#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec3 vertex_color; // vertex_color attribute

out vec3 v_color;

void main()
{
	gl_Position = vec4(position, 1.0);
	v_color = vertex_color;
}

