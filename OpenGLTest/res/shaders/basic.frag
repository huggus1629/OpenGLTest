#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D uTexWood;
uniform sampler2D uTexTroll;

void main()
{
	oColor = mix(texture(uTexWood, vTexCoord), texture(uTexTroll, vTexCoord), 0.5);
}
