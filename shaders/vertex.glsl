#version 150 core

in vec3 position;

in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 parentPos;

out vec2 TexCoords;

void main()
{
    gl_Position = proj * view * model * parentPos * vec4(position, 1.0);
	TexCoords = texCoords;
}