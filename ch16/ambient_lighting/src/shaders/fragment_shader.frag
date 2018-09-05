#version 330 core

out vec4 color;
in vec2 texCoords;

uniform vec3 box_color;
uniform vec3 ambient_light;

void main()
{
	color = vec4(ambient_light * box_color, 1.0f);
}

