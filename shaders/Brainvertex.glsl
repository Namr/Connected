#version 150 core

in vec3 position;
in vec2 texCoords;
in float id;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 parentPos;

out vec2 TexCoords;
out float ID;

void main()
{
    gl_Position = proj * view * parentPos * model * vec4(position, 1.0);
	TexCoords = texCoords;
	ID = id;
}