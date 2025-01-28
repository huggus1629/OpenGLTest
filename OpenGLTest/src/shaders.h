#pragma once
#ifndef _SHADERS_H_
#define _SHADERS_H_

unsigned int CompileShader(unsigned int type, const char* shader_source);

unsigned int CreateShader(const char* vertex_source, const char* fragment_source);

#endif
