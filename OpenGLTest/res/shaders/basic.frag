#version 330 core

layout(location = 0) out vec4 color;

in vec4 vertPos;

uniform vec3 timedColor;

void main()
{
	color = vec4(timedColor, 1.0);
	color = color + abs(vec4(vertPos.x, vertPos.x + vertPos.y, vertPos.y, 1.0));
}
