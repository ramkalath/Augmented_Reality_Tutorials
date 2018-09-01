#version 330 core

out vec4 color;
in vec2 texCoords;

uniform vec3 box_color;
uniform vec3 light_color;

void main()
{
	color = vec4(light_color * box_color, 1.0f);
}

