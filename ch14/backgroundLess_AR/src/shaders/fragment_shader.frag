#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D batman_texture;
uniform sampler2D wood_texture;

void main()
{
	color = mix(texture(wood_texture, texCoords), texture(batman_texture, texCoords), 0.5);
}

