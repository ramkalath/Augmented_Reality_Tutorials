#version 330 core

out vec4 color;
in vec3 v_color;

void main()
{
	color = vec4(v_color, 1.0f);
}

