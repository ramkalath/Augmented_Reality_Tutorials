#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute

uniform mat4 model_lamp;
uniform mat4 modelview_lamp;
uniform mat4 projection_lamp;

void main()
{
	gl_Position = projection_lamp * modelview_lamp * model_lamp * vec4(position, 1.0);
}

