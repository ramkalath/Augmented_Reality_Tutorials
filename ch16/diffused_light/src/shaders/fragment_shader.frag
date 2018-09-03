#version 330 core

out vec4 color;
in float diffuse_strength;

uniform vec3 box_color;
uniform vec3 light_color;

void main()
{
	float ambient_strength = 0.3f;
	vec3 ambient_light = ambient_strength * light_color;
	vec3 diffuse_light = diffuse_strength * light_color;

	color = vec4((ambient_light+diffuse_light) * box_color, 1.0f);
}

