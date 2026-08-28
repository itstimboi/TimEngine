#version 460 core

out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	lightColor = vec4(0.0f);

	FragColor = lightColor;
}