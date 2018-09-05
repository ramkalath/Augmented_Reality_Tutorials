#version 330 core

in vec3 diffuse_light;

uniform vec3 box_color;
uniform vec3 ambient_light;

out vec4 color;

void main()
{
	color = vec4((diffuse_light + ambient_light) * box_color, 1.0f);
}

