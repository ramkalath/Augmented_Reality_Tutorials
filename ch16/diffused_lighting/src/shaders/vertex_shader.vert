#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec3 normals; // texture_coordinate attribute

out vec3 diffuse_light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lamp_pos;
uniform vec3 light_color;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);

	vec3 norm = normalize(vec3(model * vec4(normals, 1.0f)));
	vec3 fragment_position = vec3(model * vec4(position, 1.0f));
	// we need light direction and distance from light
	vec3 light_direction = lamp_pos - fragment_position;
	float dist = distance(fragment_position, lamp_pos);

	// calculating diffused strength
	float diffuse_value = 1/pow(dist, 2) * max(dot(norm, light_direction), 0.0f);	
	diffuse_light = diffuse_value * light_color;
}

