#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec3 normal;

uniform mat4 model_suzanne;
uniform mat4 modelview_suzanne;
uniform mat4 perspective_projection_suzanne;

// matrices required for lighting
uniform vec3 lamp_pos; 
uniform vec3 light_color; 

// To be passed to fragment shader
out vec3 norm;
out vec3 fragment_position;
out vec3 diffuse_light;

void main()
{
	gl_Position = perspective_projection_suzanne * modelview_suzanne * model_suzanne * vec4(position, 1.0);

	fragment_position = vec3(model_suzanne * vec4(position, 1.0f));
	norm = normalize(vec3(model_suzanne * vec4(normal, 1.0f))); // norm should not be calculated like this if there is non-uniform scaling or translation.

	// Diffused Lighting
	vec3 light_vector = normalize(lamp_pos - fragment_position);
	float dist = distance(fragment_position, lamp_pos);
	float diffuse_value = 1/pow(dist, 2) * max(dot(norm, light_vector), 0.0f);	
	diffuse_light = diffuse_value * light_color;
} 
