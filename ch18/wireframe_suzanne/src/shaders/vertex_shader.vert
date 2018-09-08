#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * model * vec4(position, 1.0);
	//gl_Position = model * vec4(position, 1.0);
}

