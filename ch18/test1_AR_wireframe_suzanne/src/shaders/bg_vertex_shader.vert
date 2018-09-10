#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute

out vec2 texCoords;

uniform mat4 modelview_bg;
uniform mat4 perspective_projection_bg;

void main()
{
	gl_Position = perspective_projection_bg * modelview_bg * vec4(position, 1.0);
	texCoords = vec2(tex_coords.x, 1.0-tex_coords.y); // to invert the texture
}

