#version 150 core

in vec2 TexCoords;

uniform vec4 objColor;
uniform bool isTexOn;
uniform sampler3D textureArray;
uniform float layer;
uniform int MRIView;

out vec4 outColor;

void main()
{
	outColor = objColor;
	if(isTexOn)
	{
		if(MRIView == 0)
		{
			if(texture(textureArray, vec3(TexCoords.x, layer, TexCoords.y)).r == 0.0)
			{
				outColor = vec4(texture(textureArray, vec3(TexCoords.x, layer, TexCoords.y)).rrr, 0.0f);
			}
			else
			{
				outColor = vec4(texture(textureArray, vec3(TexCoords.x, layer, TexCoords.y)).rrr, 1.0f);
			}
		}
		else
		{
			if(texture(textureArray, vec3(TexCoords.x, TexCoords.y, layer)).r == 0.0)
			{
				outColor = vec4(texture(textureArray, vec3(TexCoords.x, TexCoords.y, layer)).rrr, 0.0f);
			}
			else
			{
				outColor = vec4(texture(textureArray, vec3(TexCoords.x, TexCoords.y, layer)).rrr, 1.0f);
			}
		}
	}
}