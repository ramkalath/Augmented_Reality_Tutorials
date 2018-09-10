#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec3 normal;

uniform mat4 model_suzanne;
uniform mat4 modelview_suzanne;
uniform mat4 perspective_projection_suzanne;

void main()
{
	gl_Position = perspective_projection_suzanne * modelview_suzanne * model_suzanne * vec4(position, 1.0);
}

