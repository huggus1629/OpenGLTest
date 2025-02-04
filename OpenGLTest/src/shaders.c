#include "shaders.h"
#include "filetools.h"

#include <glad/glad.h>

#include <stdlib.h>
#include <stdio.h>

unsigned int CompileShader(unsigned int type, const char* shader_source)
{
	unsigned int shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &shader_source, 0);
	glCompileShader(shader_id);

	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int len;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		char* errmsg = calloc(len, sizeof(char));
		if (!errmsg)
		{
			printf("Failed to allocate memory, exiting...\n");
			exit(EXIT_FAILURE);
		}

		glGetShaderInfoLog(shader_id, len, &len, errmsg);
		printf((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment");
		printf(" shader compilation failed!\nMore information:\n");
		printf(errmsg);
		free(errmsg);

		glDeleteShader(shader_id);
		return 0;
	}

	return shader_id;
}

unsigned int CreateShader(const char* vertex_source, const char* fragment_source)
{
	unsigned int program_id = glCreateProgram();
	unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertex_source);
	unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragment_source);

	glAttachShader(program_id, vs_id);
	glAttachShader(program_id, fs_id);
	glLinkProgram(program_id);
	glValidateProgram(program_id);

	glDeleteShader(vs_id);
	glDeleteShader(fs_id);

	return program_id;
}

unsigned int LoadShaders(const char* vShaderPath, const char* fShaderPath)
{
	const char* vShaderSource;
	const char* fShaderSource;
	
	vShaderSource = ReadTextFile(vShaderPath);
	fShaderSource = ReadTextFile(fShaderPath);

	// check if loaded correctly
	if (!vShaderSource || !fShaderSource)
	{
		puts("Error: couldn't read shader contents.");
		puts("Local variables:");
		printf("\tvShaderSource:\t0x%p\n", vShaderSource);
		printf("\tfShaderSource:\t0x%p\n", fShaderSource);
		return 0;
	}
	unsigned int shader = CreateShader(vShaderSource, fShaderSource);
	free(vShaderSource);
	free(fShaderSource);
	return shader;
}
