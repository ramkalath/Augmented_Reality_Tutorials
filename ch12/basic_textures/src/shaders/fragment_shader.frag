#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D minion_texture;

void main()
{
	color = texture(minion_texture, texCoords);
}

