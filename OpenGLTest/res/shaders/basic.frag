#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D uTexture;

void main()
{
	oColor = texture(uTexture, vTexCoord);
}
