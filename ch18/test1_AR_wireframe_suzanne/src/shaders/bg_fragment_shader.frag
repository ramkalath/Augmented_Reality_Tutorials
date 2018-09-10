#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D webcam_texture;

void main()
{
	color = texture(webcam_texture, texCoords);
}

