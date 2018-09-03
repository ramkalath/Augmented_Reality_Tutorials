#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec3 normals; // texture_coordinate attribute

out float diffuse_strength;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lamp_pos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);

	vec3 norm = normalize(vec3(model * vec4(normals, 1.0f)));
	vec3 fragment_position = vec3(model * vec4(position, 1.0f));
	//vec3 dist = 1/(sqrt((fragment_position.x-lamp_pos.x)**2 + (fragment_position.y-lamp_pos.y)**2 + (fragment_position.z-lamp_pos.z)**2));
	float dist = distance(fragment_position, lamp_pos);
	vec3 light_direction = lamp_pos - fragment_position;

	diffuse_strength = 1/pow(dist, 2) * max(dot(norm, light_direction), 0.0f);	
}

