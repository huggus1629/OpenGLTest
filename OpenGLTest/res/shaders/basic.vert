#version 330 core

layout(location = 0) in vec3 pos;

out vec4 vertPos;

void main()
{
	vertPos = vec4(pos, 1.0);
	gl_Position = vertPos;
}
