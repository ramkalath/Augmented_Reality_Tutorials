#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D arch_texture;
uniform sampler2D wood_texture;

void main()
{
	color = mix(texture(wood_texture, texCoords), texture(arch_texture, texCoords), 0.3); // mix function takes a mixture of both the textures and 0.3 here is the mix ratio.
}

