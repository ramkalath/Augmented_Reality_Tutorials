#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// matrices required for lighting
uniform vec3 lamp_pos; 
uniform vec3 light_color; 

// material uniforms
uniform vec3 material_diffuse;

// To be passed to fragment shader
out vec3 norm;
out vec3 fragment_position;
out vec3 diffuse_light;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);

	fragment_position = vec3(model * vec4(position, 1.0f));
	norm = normalize(vec3(model * vec4(normal, 1.0f))); // norm should not be calculated like this if there is non-uniform scaling or translation.

	// Diffused Lighting
	vec3 light_vector = normalize(lamp_pos - fragment_position);
	float dist = distance(fragment_position, lamp_pos);
	float diffuse_value = 1/pow(dist, 2) * max(dot(norm, light_vector), 0.0f);	
	diffuse_light = diffuse_value * (light_color * material_diffuse);
} 
