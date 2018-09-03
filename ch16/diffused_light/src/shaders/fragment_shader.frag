#version 330 core

out vec4 color;
in vec2 texCoords;

uniform vec3 box_color;
uniform vec3 light_color;

void main()
{
	float ambient_strength = 0.6f;
	vec3 ambient_light = ambient_strength * light_color;

	color = vec4(ambient_light * box_color, 1.0f);
}

