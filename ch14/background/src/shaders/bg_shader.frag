#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D webcam_texture;
uniform mat4 modelview;

void main()
{
	color = texture(webcam_texture, texCoords);
}

