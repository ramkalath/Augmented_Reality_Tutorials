#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection_orthographic;

void main()
{
	gl_Position = projection_orthographic * view * model * vec4(position, 1.0);
	texCoords = vec2(tex_coords.x, 1.0-tex_coords.y); // to invert the texture
}

