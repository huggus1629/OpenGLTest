#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include <GLFW/glfw3.h>

#include "camera.h"

typedef struct mouse_delta
{
	float dx;
	float dy;
} MouseDelta;

void ProcessKBInput(GLFWwindow* window, Camera* cam);

void Clbk_Mouse(GLFWwindow* window, double xpos, double ypos);

#endif
